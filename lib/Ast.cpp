#include "lib/Ast.hpp"
#include "lib/Util.hpp"
#include <format>

std::string loxlang::ast::astTypeName(AstType type) {
  std::string asText = R"ENUM(
    AssignExpr,
BinaryExpr,
CallExpr,
GetExpr,
GroupingExpr,
LiteralExpr,
LogicalExpr,
SetExpr,
SuperExpr,
ThisExpr,
UnaryExpr,
VariableExpr,
BlockStmt,
ClassStmt,
ExpressionStmt,
FunctionStmt,
IfStmt,
PrintStmt,
ReturnStmt,
VarStmt,
WhileStmt,
    )ENUM";
  return util::enumName(asText, static_cast<std::size_t>(type));
}

namespace {

using namespace loxlang::ast;

} // namespace

std::string loxlang::ast::Ast::stringify() { return std::string(); }