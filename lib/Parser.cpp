#include "lib/Parser.hpp"
#include "Parser.hpp"
#include "lib/Ast.hpp"
#include "lib/Error.hpp"
#include <memory>
#include <optional>
#include <print>
#include <span>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace loxlang;
using namespace loxlang::scan;
using namespace loxlang::parse;
using namespace loxlang::ast;

namespace {

struct ParserPanic : public std::logic_error {
  ParserPanic() : std::logic_error("parser unwinding") {}
};

class Parser {
public:
  Parser(Program &program, scan::Scanner &scanner)
      : program{program}, scanner{scanner} {}

  std::unique_ptr<Ast> parse();

  bool isAtEnd();
  scan::Token peek();
  scan::Token peekNext();
  scan::Token previous();
  bool checkNext(scan::Token::Type type);
  scan::Token advance();
  bool advanceIf(std::span<scan::Token::Type> types);
  scan::Token expect(scan::Token::Type type, std::string_view errorMsg);
  void expectPanic(scan::Token::Type type, std::string_view errorMsg);
  void error(scan::Token source, std::string_view errorMsg);
  void errorPanic(scan::Token source, std::string_view errorMsg);

private:
  Program &program;
  scan::Scanner &scanner;
  std::optional<scan::Token> prev = std::nullopt;
  std::optional<scan::Token> current = std::nullopt;
  std::optional<scan::Token> next = std::nullopt;
};

bool Parser::isAtEnd() { return peek().type != Token::Type::Eof; }

Token Parser::peek() {
  if (!current.has_value()) {
    current = scanner.next();
  }
  return current.value();
}

[[maybe_unused]] Token Parser::peekNext() {
  if (!next.has_value()) {
    next = scanner.next();
  }
  return next.value();
}

[[maybe_unused]] Token Parser::previous() {
  if (!prev.has_value()) {
    lox_fail("call to previous without ever calling advance before");
  }
  return prev.value();
}

bool Parser::checkNext(Token::Type type) {
  if (isAtEnd()) {
    return false;
  }
  return peek().type == type;
}

Token Parser::advance() {
  Token t = peek();
  prev = current;
  current = next;
  next = std::nullopt;
  return t;
}

[[maybe_unused]] bool Parser::advanceIf(std::span<Token::Type> types) {
  for (Token::Type type : types) {
    if (checkNext(type)) {
      advance();
      return true;
    }
  }
  return false;
}

Token Parser::expect(Token::Type type, std::string_view errorMsg) {
  Token t = advance();
  if (t.type != type) {
    error(t, errorMsg);
  }
  return t;
}

[[maybe_unused]] void Parser::expectPanic(Token::Type type,
                                          std::string_view errorMsg) {
  if (checkNext(type)) {
    advance();
  } else {
    error(peek(), errorMsg);
  }
}

void Parser::error(Token source, std::string_view errorMsg) {
  program.error(errorMsg, source.text);
}

void Parser::errorPanic(Token source, std::string_view errorMsg) {
  error(source, errorMsg);
  throw ParserPanic();
}

enum class BindingPower : std::uint8_t {
  None,
  AssignRight,
  AssignLeft,
  OrLeft,
  OrRight,
  AndLeft,
  AndRight,
  EqualityLeft,
  EqualityRight,
  ComparisonLeft,
  ComparisonRight,
  AddLeft,
  AddRight,
  MulLeft,
  MulRight,
  Unary,
  Call,
  Primary,
};

using PrefixParseFn = std::unique_ptr<Ast>(Parser &p);
using InfixParseFn = std::unique_ptr<Ast>(Parser &p, std::unique_ptr<Ast> left);

struct ParseRule {
  PrefixParseFn *prefix;
  InfixParseFn *infix;
  BindingPower left;
  BindingPower right;
};

std::vector<ParseRule> constructEmptyTable() {
  std::vector<Token::Type> tokens = {
      Token::Type::LPar,    Token::Type::RPar,  Token::Type::LBrace,
      Token::Type::RBrace,  Token::Type::Comma, Token::Type::Dot,
      Token::Type::Minus,   Token::Type::Plus,  Token::Type::SemiColon,
      Token::Type::Slash,   Token::Type::Star,  Token::Type::Bang,
      Token::Type::BangEq,  Token::Type::Eq,    Token::Type::EqEq,
      Token::Type::Greater, Token::Type::Less,  Token::Type::GreaterEq,
      Token::Type::LessEq,  Token::Type::Ident, Token::Type::String,
      Token::Type::Number,  Token::Type::And,   Token::Type::Class,
      Token::Type::Else,    Token::Type::False, Token::Type::Fun,
      Token::Type::For,     Token::Type::If,    Token::Type::Nil,
      Token::Type::Or,      Token::Type::Print, Token::Type::Return,
      Token::Type::Super,   Token::Type::This,  Token::Type::True,
      Token::Type::Var,     Token::Type::While, Token::Type::Eof,
      Token::Type::Err};
  std::vector<ParseRule> table;
  table.resize(tokens.size());
  for (Token::Type type : tokens) {
    std::size_t idx = static_cast<std::size_t>(type);
    table[idx] =
        ParseRule(nullptr, nullptr, BindingPower::None, BindingPower::None);
  }
  return table;
}

void wordStart(std::vector<ParseRule> &table, Token::Type type,
               PrefixParseFn *fn) {
  std::size_t idx = static_cast<std::size_t>(type);
  table[idx].prefix = fn;
}

void wordContinue(std::vector<ParseRule> &table, Token::Type type,
                  InfixParseFn *fn, BindingPower left, BindingPower right) {
  std::size_t idx = static_cast<std::size_t>(type);
  ParseRule &r = table[idx];
  r.infix = fn;
  r.left = left;
  r.right = right;
}

std::vector<ParseRule> computeParseTable();

ParseRule findRule(Token::Type type) {
  static std::vector<ParseRule> parseTable = computeParseTable();
  std::size_t index = static_cast<std::size_t>(type);
  return parseTable[index];
}

std::unique_ptr<Ast> expressionUntil(Parser &p, BindingPower minPower) {
  PrefixParseFn *prefixRule = findRule(p.peek().type).prefix;
  if (prefixRule == nullptr) {
    p.errorPanic(p.peek(), "Expected expression start");
  }
  std::unique_ptr<Ast> expr = prefixRule(p);

  while (true) {
    Token op = p.peek();
    ParseRule rule = findRule(op.type);
    if (rule.infix == nullptr) {
      // If we hit a bad character, give up and let the caller handle the
      // problem
      break;
    }
    if (rule.left < minPower) {
      break;
    }

    expr = rule.infix(p, std::move(expr));
  }

  return expr;
}

std::unique_ptr<Ast> expression(Parser &p) {
  return expressionUntil(p, BindingPower::None);
}

std::unique_ptr<Ast> nilLiteral(Parser &p) {
  Token literal = p.advance();
  lox_assert_eq(literal.type, Token::Type::Nil, "Should be Nil keyword");
  return std::make_unique<Literal>(Value(nullptr));
}

std::unique_ptr<Ast> trueLiteral(Parser &p) {
  Token literal = p.advance();
  lox_assert_eq(literal.type, Token::Type::True, "Should be True keyword");
  return std::make_unique<Literal>(Value(true));
}

std::unique_ptr<Ast> falseLiteral(Parser &p) {
  Token literal = p.advance();
  lox_assert_eq(literal.type, Token::Type::False, "Should be False keyword");
  return std::make_unique<Literal>(Value(false));
}

std::unique_ptr<Ast> numberLiteral(Parser &p) {
  Token literal = p.advance();
  lox_assert_eq(literal.type, Token::Type::Number, "Should be Number Literal");
  std::string ownedStr = std::string(literal.text);
  Value loxValue = 0.0;
  try {
    loxValue = std::stod(ownedStr);
  } catch (std::invalid_argument &) {
    p.error(literal, "Could not parse this number");
  } catch (std::out_of_range &) {
    p.error(literal, "Number literal is out of range for Lox number (IEEE 754 "
                     "double precision floating point)");
  }
  return std::make_unique<Literal>(loxValue);
}

std::unique_ptr<Ast> stringLiteral(Parser &p) {
  Token literal = p.advance();
  lox_assert_eq(literal.type, Token::Type::String, "Should be String Literal");
  Value loxValue = std::string(literal.text);
  return std::make_unique<Literal>(loxValue);
}

std::unique_ptr<Ast> variableUse(Parser &p) {
  Token name = p.advance();
  lox_assert_eq(name.type, Token::Type::Ident, "Should be Identifier");
  return std::make_unique<Variable>(name);
}

std::unique_ptr<Ast> unary(Parser &p) {
  Token op = p.advance();
  if (op.type != Token::Type::Minus && op.type != Token::Type::Bang) {
    lox_fail("bad unary operator. Possible are '-', '!'");
  }
  return std::make_unique<Unary>(op, expressionUntil(p, BindingPower::Unary));
}

std::unique_ptr<Ast> binary(Parser &p, std::unique_ptr<Ast> left) {
  Token op = p.advance();
  ParseRule rule = findRule(op.type);
  return std::make_unique<Binary>(std::move(left), op,
                                  expressionUntil(p, rule.right));
}

std::unique_ptr<Ast> grouping(Parser &p) {
  p.expect(Token::Type::LPar, "expecting opening '('");
  std::unique_ptr<Ast> expr = expression(p);
  p.expect(Token::Type::RPar, "expected closing ')'");
  return std::make_unique<Grouping>(std::move(expr));
}

std::vector<ParseRule> computeParseTable() {
  std::vector<ParseRule> t = constructEmptyTable();

  wordStart(t, Token::Type::Nil, nilLiteral);
  wordStart(t, Token::Type::True, trueLiteral);
  wordStart(t, Token::Type::False, falseLiteral);
  wordStart(t, Token::Type::Number, numberLiteral);
  wordStart(t, Token::Type::String, stringLiteral);
  wordStart(t, Token::Type::Ident, variableUse);
  wordStart(t, Token::Type::Minus, unary);
  wordStart(t, Token::Type::Bang, unary);
  wordStart(t, Token::Type::LPar, grouping);

  wordContinue(t, Token::Type::Plus, binary, BindingPower::AddLeft,
               BindingPower::AddRight);
  wordContinue(t, Token::Type::Minus, binary, BindingPower::AddLeft,
               BindingPower::AddRight);
  wordContinue(t, Token::Type::Star, binary, BindingPower::MulLeft,
               BindingPower::MulRight);
  wordContinue(t, Token::Type::Slash, binary, BindingPower::MulLeft,
               BindingPower::MulRight);

  wordContinue(t, Token::Type::EqEq, binary, BindingPower::EqualityLeft,
               BindingPower::EqualityRight);
  wordContinue(t, Token::Type::BangEq, binary, BindingPower::EqualityLeft,
               BindingPower::EqualityRight);
  wordContinue(t, Token::Type::Greater, binary, BindingPower::ComparisonLeft,
               BindingPower::ComparisonRight);
  wordContinue(t, Token::Type::GreaterEq, binary, BindingPower::ComparisonLeft,
               BindingPower::ComparisonRight);
  wordContinue(t, Token::Type::Less, binary, BindingPower::ComparisonLeft,
               BindingPower::ComparisonRight);
  wordContinue(t, Token::Type::LessEq, binary, BindingPower::ComparisonLeft,
               BindingPower::ComparisonRight);

  wordContinue(t, Token::Type::Eq, binary, BindingPower::AssignLeft,
               BindingPower::AssignRight);

  return t;
}

std::unique_ptr<Ast> Parser::parse() {
  try {
    return expression(*this);
  } catch (ParserPanic &) {
    std::println("Parser panicked, cannot produce Abstract Syntax Tree.");
    return nullptr;
  }
}

} // namespace

std::unique_ptr<Ast> parse::parse(Program &p, Scanner &s) {
  return Parser(p, s).parse();
}