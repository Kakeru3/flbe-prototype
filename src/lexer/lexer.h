#pragma once
#ifndef LEXER_H
#define LEXER_H

#include <string>

enum Token {
  tok_eof = -1,

  // commands
  tok_fun = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  // control
  tok_if = -6,
  tok_then = -7,
  tok_else = -8,
  tok_for = -9,
  tok_in = -10,

  // operators
  tok_binary = -11,
  tok_unary = -12,

  // var definition
  tok_var = -13,

  // return
  tok_return = -14,

  tok_char = -100,
  
  tok_u8 = -101,
  tok_u16 = -102,
  tok_u32 = -103,
  tok_u64 = -104,

  tok_i8 = -105,
  tok_i16 = -106,
  tok_i32 = -107,
  tok_i64 = -108,

  tok_f32 = -109,
  tok_f64 = -110,

  tok_bool = -111,
  tok_vec = -112,
  tok_void = -113,

  // one-char symbols:
  /// ";"
  tok_semi = 59,
  /// ","
  tok_comma = 44,
  /// "."
  tok_dot = 46,
  /// "("
  tok_openParen = 40,
  /// ")"
  tok_closeParen = 41,
  /// "{"
  tok_openBrace = 123,
  /// "}"
  tok_closeBrace = 125,
  /// "["
  tok_openBracket = 91,
  /// "]"
  tok_closeBracket = 93,
  /// "@"
  tok_at = 64,
  /// "#"
  tok_pound = 35,
  /// "~"
  tok_tilde = 126,
  /// "?"
  tok_question = 63,
  /// ":"
  tok_colon = 58,
  /// "$"
  tok_dollar = 36,
  /// "="
  tok_eq = 61,
  /// "!"
  tok_bang = 33,
  /// "<"
  tok_lt = 60,
  /// ">"
  tok_gt = 62,
  /// "-"
  tok_minus = 45,
  /// "&"
  tok_and = 38,
  /// "|"
  tok_or = 47,
  /// "+"
  tok_plus = 43,
  /// "*"
  tok_star = 42,
  /// "/"
  tok_slash = 47,
  /// "^"
  tok_caret = 94,
  /// "%"
  tok_percent = 37,
  /// '"'
  tok_doubleQuote = 34,
  /// "'"
  tok_quote = 39,

  // multiple-char symbols:
  /// "=="
  tok_eo = -15,
  /// "+="
  tok_aa = -16,
  /// "-="
  tok_sa = -17,
  /// "*="
  tok_ma = -18,
  /// "/="
  tok_da = -19,
  /// "%="
  tok_ra = -20,

  tok_unknown = -99
};

extern std::string IdentifierStr;
extern double NumVal;   

int gettok();
std::string getidentifier(bool isString);

#endif