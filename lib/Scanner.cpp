#include "lib/Scanner.hpp"
#include "lib/Error.hpp"
#include "lib/Util.hpp"
#include <array>
#include <cctype>

namespace {

bool isDigit(char c) { return '0' <= c && c <= '9'; }
bool isBlank(char c) { return std::isblank(static_cast<unsigned char>(c)); }
bool isIdentStart(char c) {
  return c == '_' || std::isalpha(static_cast<unsigned char>(c));
}
bool isIdentPart(char c) {
  return c == '_' || std::isalnum(static_cast<unsigned char>(c));
}

loxlang::scan::Token::Type cmp(loxlang::scan::Token::Type type,
                               std::string_view id, std::string_view actual) {
  if (id == actual) {
    return type;
  } else {
    return loxlang::scan::Token::Type::Ident;
  }
}

loxlang::scan::Token::Type potentialKeyword(std::string_view identifier) {
  using loxlang::scan::Token;
  switch (identifier.size()) {
  case 2:
    switch (identifier[0]) {
    case 'i': return cmp(Token::Type::If, "if", identifier);
    case 'o': return cmp(Token::Type::Or, "or", identifier);
    default: return Token::Type::Ident;
    }
    break;
  case 3:
    switch (identifier[1]) {
    case 'n': return cmp(Token::Type::And, "and", identifier);
    case 'o': return cmp(Token::Type::For, "for", identifier);
    case 'u': return cmp(Token::Type::Fun, "fun", identifier);
    case 'i': return cmp(Token::Type::Nil, "nil", identifier);
    case 'a': return cmp(Token::Type::Var, "var", identifier);
    default: return Token::Type::Ident;
    }
    break;
  case 4:
    switch (identifier[1]) {
    case 'l': return cmp(Token::Type::Else, "else", identifier);
    case 'h': return cmp(Token::Type::This, "this", identifier);
    case 'r': return cmp(Token::Type::True, "true", identifier);
    default: return Token::Type::Ident;
    }
    break;
  case 5:
    switch (identifier[1]) {
    case 'c': return cmp(Token::Type::Class, "class", identifier);
    case 'f': return cmp(Token::Type::False, "false", identifier);
    case 'p': return cmp(Token::Type::Print, "print", identifier);
    case 's': return cmp(Token::Type::Super, "super", identifier);
    case 'w': return cmp(Token::Type::While, "while", identifier);
    default: return Token::Type::Ident;
    }
    break;
  case 6: return cmp(Token::Type::Return, "retrun", identifier);

  default: return Token::Type::Ident;
  }
}

bool isLoxChar(char c) {
  switch (c) {
  case '(':
  case ')':
  case '{':
  case '}':
  case ',':
  case '.':
  case '-':
  case '+':
  case ';':
  case '*':
  case '!':
  case '=':
  case '<':
  case '>':
  case '/':
  case '\"':
  case '\0':
  case '_': return true;
  default: return isBlank(c) || isDigit(c) || isIdentPart(c) || isIdentStart(c);
  }
}

} // namespace

loxlang::scan::Token loxlang::scan::Scanner::next() {
  while (true) {
    char c = advance();
    switch (c) {
    case '(': return token(Token::Type::LPar);
    case ')': return token(Token::Type::RPar);
    case '{': return token(Token::Type::LBrace);
    case '}': return token(Token::Type::RBrace);
    case ',': return token(Token::Type::Comma);
    case '.': return token(Token::Type::Dot);
    case '-': return token(Token::Type::Minus);
    case '+': return token(Token::Type::Plus);
    case ';': return token(Token::Type::SemiColon);
    case '*': return token(Token::Type::Star);

    case '!':
      return token(advanceIf('=') ? Token::Type::BangEq : Token::Type::Bang);
    case '=':
      return token(advanceIf('=') ? Token::Type::EqEq : Token::Type::Eq);
    case '<':
      return token(advanceIf('=') ? Token::Type::LessEq : Token::Type::Less);
    case '>':
      return token(advanceIf('=') ? Token::Type::GreaterEq
                                  : Token::Type::Greater);

    case '/': {
      if (advanceIf('/')) {
        lineComment();
      } else if (advanceIf('*')) {
        blockComment();
      } else {
        return token(Token::Type::Slash);
      }
    } break;

    case '\"': return string();

    case '\0': return token(Token::Type::Eof);

    default: {
      if (isBlank(c)) {
        discard();
      } else if (isDigit(c)) {
        return number();
      } else if (isIdentStart(c)) {
        return identifierOrKeyword();
      } else {
        unknownChars();
      }
    } break;
    }
  }
}

void loxlang::scan::Scanner::lineComment() {
  while (peek() != '\n' && !isAtEnd()) {
    advance();
  }
  discard();
}

void loxlang::scan::Scanner::blockComment() {
  std::size_t nesting = 1;
  while (nesting > 0) {
    if (isAtEnd()) {
      error("unterminated comment");
      return;
    }
    if (peek() == '/' && peekNext() == '*') {
      nesting++;
    }
    if (peek() == '*' && peekNext() == '/') {
      nesting--;
    }
    advance();
  }
  lox_assert_eq(advance(), '/', "ending slash");

  discard();
}

loxlang::scan::Token loxlang::scan::Scanner::string() {
  while (peek() != '\"') {
    if (isAtEnd()) {
      return error("unterminated string literal");
    }

    advance();
  }
  lox_assert_eq(advance(), '\"', "Didn’t read closing ‘\"’");
  return token(Token::Type::String);
}

loxlang::scan::Token loxlang::scan::Scanner::number() {
  while (isDigit(peek())) {
    advance();
  }

  if (peek() == '.' && isDigit(peekNext())) {
    lox_assert_eq(advance(), '.', "consume '.'");
    while (isDigit(peek())) {
      advance();
    }
  }

  return token(Token::Type::Number);
}

loxlang::scan::Token loxlang::scan::Scanner::identifierOrKeyword() {
  while (isIdentPart(peek())) {
    advance();
  }

  Token t = token(Token::Type::Ident);
  t.type = potentialKeyword(t.text);
  return t;
}

loxlang::scan::Token loxlang::scan::Scanner::unknownChars() {
  while (!isLoxChar(peek())) {
    advance();
  }
  return error("unknown character");
}

bool loxlang::scan::Scanner::isAtEnd() const {
  return current >= program.programText().size();
}

char loxlang::scan::Scanner::peek() {
  if (isAtEnd()) {
    return '\0';
  }

  return program.programText()[current];
}

char loxlang::scan::Scanner::peekNext() {
  if (current + 1 >= program.programText().size()) {
    return '\0';
  }
  return program.programText()[current + 1];
}

char loxlang::scan::Scanner::advance() {
  char c = peek();
  current++;
  return c;
}

bool loxlang::scan::Scanner::advanceIf(char expected) {
  if (peek() == expected) {
    advance();
    return true;
  }
  return false;
}

loxlang::scan::Token loxlang::scan::Scanner::token(Token::Type type) {
  std::string_view text = program.part(start, current - start);
  Token t = Token(type, text);
  start = current;
  return t;
}

void loxlang::scan::Scanner::discard() { token(Token::Type::Err); }

loxlang::scan::Token loxlang::scan::Scanner::error(std::string_view msg) {
  Token t = token(Token::Type::Err);
  program.error(msg, t.text);
  return t;
}

std::string loxlang::scan::Token::typeName(Type type) {
  std::string_view asText = R"ENUMS(
    LPar, RPar, LBrace, RBrace,
    Comma, Dot, Minus, Plus, SemiColon, Slash, Star,
    Bang, BangEq, Eq, EqEq, Greater, Less, GreaterEq, LessEq,
    Ident, String, Number,
    And, Class, Else, False, Fun, For, If, Nil, Or, Print, 
    Return, Super, This, True, Var, While,

    Eof, Err
  )ENUMS";
  return util::enumName(asText, static_cast<std::size_t>(type));
}
