#pragma once

#include "lib/Program.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace loxlang::scan {

struct Token {
  enum class Type {
    // clang-format off
    LPar, RPar, LBrace, RBrace,
    Comma, Dot, Minus, Plus, SemiColon, Slash, Star,
    Bang, BangEq, Eq, EqEq, Greater, Less, GreaterEq, LessEq,
    Ident, String, Number,
    And, Class, Else, False, Fun, For, If, Nil, Or, Print, 
    Return, Super, This, True, Var, While,

    Eof, Err
    //clang-format on
  };
  Type type;
  std::string_view text;

  static std::string typeName(Type type);
};

struct Scanner {
  Scanner(Program &p) : program{p} {}

  Token next();

private:
  void lineComment();
  void blockComment();
  Token string();
  Token number();
  Token identifierOrKeyword();
  Token unknownChars();
  bool isAtEnd() const;
  char peek();
  char peekNext();
  char advance();
  bool advanceIf(char expected);
  Token token(Token::Type type);
  void discard();
  Token error(std::string_view msg);


  Program &program;
  std::size_t start = 0;
  std::size_t current = 0;
};

} // namespace loxlang::scan
