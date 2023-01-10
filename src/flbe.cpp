#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "ast/ast.h"
#include "codegen/codegen.h"
#include "parser/parser.h"
#include "lexer/lexer.h"

void InitializeModuleAndPassManager()
{
  // Open a new module.
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("my cool jit", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

void HandleDefinition()
{
  if (auto FnAST = ParseDefinition())
  {
    if (auto *FnIR = FnAST->codegen())
    {
      fprintf(stderr, "Read function definition:");
      FnIR->print(errs());
      fprintf(stderr, "\n");
    }
  }
  else
  {
    // Skip token for error recovery.
    getNextToken();
  }
}

void HandleExtern()
{
  if (auto ProtoAST = ParseExtern())
  {
    if (auto *FnIR = ProtoAST->codegen())
    {
      fprintf(stderr, "Read extern: ");
      FnIR->print(errs());
      fprintf(stderr, "\n");
      FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
    }
  }
  else
  {
    // Skip token for error recovery.
    getNextToken();
  }
}

void HandleTopLevelExpression()
{
  // Evaluate a top-level expression into an anonymous function.
  if (auto FnAST = ParseTopLevelExpr())
  {
    FnAST->codegen();
  }
  else
  {
    // Skip token for error recovery.
    getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
void MainLoop()
{
  while (true)
  {
    switch (CurTok)
    {
    case tok_eof:
      return;
    /*case ';': // ignore top-level semicolons.
      getNextToken();
      break;*/
    case tok_fun:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main()
{
  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence[tok_eq]      = 2;
  BinopPrecedence[tok_aa]      = 2;
  BinopPrecedence[tok_sa]      = 2;
  BinopPrecedence[tok_ma]      = 2;
  BinopPrecedence[tok_da]      = 2;
  BinopPrecedence[tok_ra]      = 2;
  BinopPrecedence[tok_lt]      = 10;
  BinopPrecedence[tok_eq]      = 10;
  BinopPrecedence[tok_plus]    = 20;
  BinopPrecedence[tok_minus]   = 20;
  BinopPrecedence[tok_star]    = 40; // highest.
  BinopPrecedence[tok_slash]   = 40; // highest.
  BinopPrecedence[tok_percent] = 40; // highest.

  getNextToken();

  InitializeModuleAndPassManager();

  // Run the main "interpreter loop" now.
  MainLoop();

  // Initialize the target registry etc.
  InitializeAllTargetInfos();
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmParsers();
  InitializeAllAsmPrinters();

  auto TargetTriple = sys::getDefaultTargetTriple();
  TheModule->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target)
  {
    errs() << Error;
    return 1;
  }

  auto CPU = "generic";
  auto Features = "";

  TargetOptions opt;
  auto RM = Optional<Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  TheModule->setDataLayout(TheTargetMachine->createDataLayout());

  auto Filename = "output.o";
  std::error_code EC;
  raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

  if (EC)
  {
    errs() << "Could not open file: " << EC.message();
    return 1;
  }

  legacy::PassManager pass;
  auto FileType = CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
  {
    errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*TheModule);
  dest.flush();

  outs() << "Wrote " << Filename << "\n";

  return 0;
}