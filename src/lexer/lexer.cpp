#include <iostream>
#include <cctype>
#include <string>
#include "lexer.h"

static int LastChar = ' ';

std::string IdentifierStr; // Filled in if tok_identifier
double NumVal;             // Filled in if tok_number

/// gettok - Return the next token from standard input.
int gettok()
{
  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar))
  { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "fn")
      return tok_fun;
    if (IdentifierStr == "extern")
      return tok_extern;
    if (IdentifierStr == "if")
      return tok_if;
    if (IdentifierStr == "then")
      return tok_then;
    if (IdentifierStr == "else")
      return tok_else;
    if (IdentifierStr == "for")
      return tok_for;
    if (IdentifierStr == "in")
      return tok_in;
    if (IdentifierStr == "binary")
      return tok_binary;
    if (IdentifierStr == "unary")
      return tok_unary;
    if (IdentifierStr == "var")
      return tok_var;
    if (IdentifierStr == "return")
      return tok_return;

    if (IdentifierStr == "char")
      return tok_char;
    if (IdentifierStr == "u8")
      return tok_u8;
    if (IdentifierStr == "u16")
      return tok_u16;
    if (IdentifierStr == "u32")
      return tok_u32;
    if (IdentifierStr == "u64")
      return tok_u64;
    if (IdentifierStr == "i8")
      return tok_i8;
    if (IdentifierStr == "i16")
      return tok_i16;
    if (IdentifierStr == "i32")
      return tok_i32;
    if (IdentifierStr == "i64")
      return tok_i64;
    if (IdentifierStr == "f32")
      return tok_f32;
    if (IdentifierStr == "f64")
      return tok_f64;
    if (IdentifierStr == "bool")
      return tok_bool;
    if (IdentifierStr == "vec")
      return tok_vec;
    if (IdentifierStr == "void")
      return tok_void;

    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.')
  { // Number: [0-9.]+
    std::string NumStr;
    do
    {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }

  if (ispunct(LastChar))
  {
    int ThisChar = LastChar;
    LastChar = getchar();

    if (ThisChar == ';')
      return tok_semi;
    if (ThisChar == ',')
      return tok_comma;
    /*if (LastChar == '.')
      return Dot;*/
    if (ThisChar == '(')
      return tok_openParen;
    if (ThisChar == ')')
      return tok_closeParen;
    if (ThisChar == '{')
      return tok_openBrace;
    if (ThisChar == '}')
      return tok_closeBrace;
    if (ThisChar == '[')
      return tok_openBracket;
    if (ThisChar == ']')
      return tok_closeBracket;
    if (ThisChar == '@')
      return tok_at;
    if (ThisChar == '#')
      return tok_pound;
    if (ThisChar == '~')
      return tok_tilde;
    if (ThisChar == '?')
      return tok_question;
    if (ThisChar == ':')
      return tok_colon;
    if (ThisChar == '$')
      return tok_dollar;
    if (ThisChar == '=')
    {
      if (LastChar == '=')
      {
        LastChar = getchar();
        return tok_eo;
      }
      else
        return tok_eq;
    }
    if (ThisChar == '!')
      return tok_bang;
    if (ThisChar == '<')
      return tok_lt;
    if (ThisChar == '>')
      return tok_gt;
    if (ThisChar == '-')
    {
      if (LastChar == '=')
      {
        LastChar = getchar();
        return tok_sa;
      }
      else
        return tok_minus;
    }
    if (ThisChar == '&')
      return tok_and;
    if (ThisChar == '|')
      return tok_or;
    if (ThisChar == '+')
    {
      if (LastChar == '=')
      {
        LastChar = getchar();
        return tok_aa;
      }
      else
        return tok_plus;
    }
    if (ThisChar == '*')
    {
      if (LastChar == '=')
      {
        LastChar = getchar();
        return tok_ma;
      }
      else
        return tok_star;
    }
    if (ThisChar == '/')
    {
      if (LastChar == '/')
      {
        do
          LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
          return gettok();
      }
      if (LastChar == '=')
      {
        LastChar = getchar();
        return tok_da;
      }
      else
        return tok_slash;
    }
    if (ThisChar == '^')
      return tok_caret;
    if (ThisChar == '%')
    {
      if (LastChar == '=')
      {
        LastChar = getchar();
        return tok_ra;
      }
      else
        return tok_percent;
    }
    if (ThisChar == '"')
      return tok_doubleQuote;
    if (ThisChar == '\'')
      return tok_quote;

    return tok_unknown;
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

std::string getidentifier(bool isString)
{
  IdentifierStr = LastChar;
  LastChar = getchar();
  while (isString ? LastChar != tok_doubleQuote : LastChar != tok_quote)
  {
    IdentifierStr += LastChar;
    LastChar = getchar();
  }
  
  return IdentifierStr;
}