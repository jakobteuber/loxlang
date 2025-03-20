#include "lib/Scanner.hpp"
#include "lib/Program.hpp"
#include "gtest/gtest.h"

using namespace loxlang::scan;
using namespace loxlang;

TEST(Scanner, SimpleTokens) {
  auto program = Program(
      "test",
      "(){},.-+;/ *! != = == > >= < <= myFavVarName \"HelloWorld\" 33 33.7 "
      "and class else false fun for if nil or print return super this true var "
      "while" /*Eof*/);
  auto scanner = Scanner(program);
  EXPECT_EQ(scanner.next().type, Token::Type::LPar);
  EXPECT_EQ(scanner.next().type, Token::Type::RPar);
  EXPECT_EQ(scanner.next().type, Token::Type::LBrace);
  EXPECT_EQ(scanner.next().type, Token::Type::RBrace);
  EXPECT_EQ(scanner.next().type, Token::Type::Comma);
  EXPECT_EQ(scanner.next().type, Token::Type::Dot);
  EXPECT_EQ(scanner.next().type, Token::Type::Minus);
  EXPECT_EQ(scanner.next().type, Token::Type::Plus);
  EXPECT_EQ(scanner.next().type, Token::Type::SemiColon);
  EXPECT_EQ(scanner.next().type, Token::Type::Slash);
  EXPECT_EQ(scanner.next().type, Token::Type::Star);
  EXPECT_EQ(scanner.next().type, Token::Type::Bang);
  EXPECT_EQ(scanner.next().type, Token::Type::BangEq);
  EXPECT_EQ(scanner.next().type, Token::Type::Eq);
  EXPECT_EQ(scanner.next().type, Token::Type::EqEq);
  EXPECT_EQ(scanner.next().type, Token::Type::Greater);
  EXPECT_EQ(scanner.next().type, Token::Type::GreaterEq);
  EXPECT_EQ(scanner.next().type, Token::Type::Less);
  EXPECT_EQ(scanner.next().type, Token::Type::LessEq);
  EXPECT_EQ(scanner.next().type, Token::Type::Ident);
  EXPECT_EQ(scanner.next().type, Token::Type::String);
  EXPECT_EQ(scanner.next().type, Token::Type::Number);
  EXPECT_EQ(scanner.next().type, Token::Type::Number);
  EXPECT_EQ(scanner.next().type, Token::Type::And);
  EXPECT_EQ(scanner.next().type, Token::Type::Class);
  EXPECT_EQ(scanner.next().type, Token::Type::Else);
  EXPECT_EQ(scanner.next().type, Token::Type::False);
  EXPECT_EQ(scanner.next().type, Token::Type::Fun);
  EXPECT_EQ(scanner.next().type, Token::Type::For);
  EXPECT_EQ(scanner.next().type, Token::Type::If);
  EXPECT_EQ(scanner.next().type, Token::Type::Nil);
  EXPECT_EQ(scanner.next().type, Token::Type::Or);
  EXPECT_EQ(scanner.next().type, Token::Type::Print);
  EXPECT_EQ(scanner.next().type, Token::Type::Return);
  EXPECT_EQ(scanner.next().type, Token::Type::Super);
  EXPECT_EQ(scanner.next().type, Token::Type::This);
  EXPECT_EQ(scanner.next().type, Token::Type::True);
  EXPECT_EQ(scanner.next().type, Token::Type::Var);
  EXPECT_EQ(scanner.next().type, Token::Type::While);
  EXPECT_EQ(scanner.next().type, Token::Type::Eof);
}