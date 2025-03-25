#ifndef LOXLANG_LIB_SCANNER_HPP
#define LOXLANG_LIB_SCANNER_HPP

#include "lib/Program.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace loxlang::scan {

/**
 * @brief Representation of a program token.
 */
struct Token {
  /**
   * @brief Token Types for Lox
   */
  enum class Type : std::uint8_t {
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

  /**
   * @brief The token type
   */
  Type type;

  /**
   * @brief A view into the program text that is represented by this token.
   */
  std::string_view text;

  /**
   * @brief Retrieves the name of a program type
   */
  static std::string typeName(Type type);
};


/**
 * @brief The Scanner or Tokenizer for Lox
 */
struct Scanner {
  explicit Scanner(Program &p) : program{p} {}

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

#endif
