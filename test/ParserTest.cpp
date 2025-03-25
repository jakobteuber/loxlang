#include "lib/Parser.hpp"
#include "lib/Ast.hpp"
#include "lib/Program.hpp"
#include "lib/Scanner.hpp"
#include "gtest/gtest.h"
#include <string_view>

using namespace loxlang;
using namespace loxlang::scan;

TEST(Parser, SimpleExpressions) {
  std::string_view text = "(5 - (3 - 1)) + -1";
  Program p = Program("ParserTest", text);
  Scanner s = Scanner(p);
  auto ast = parse::parse(p, s);
  std::string asString = ast->stringify();
  ASSERT_EQ(asString, "(+ (grouping (- 5 (grouping (- 3 1)))) (- 1))");
}