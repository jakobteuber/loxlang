#include "lib/Ast.hpp"
#include "lib/Util.hpp"
#include <format>
#include <sstream>

using namespace loxlang::ast;
using namespace loxlang::util;

std::string astTypeName(AstType type) {
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
  return enumName(asText, static_cast<std::size_t>(type));
}

namespace {

struct StringifyVisitor : public Visitor<void> {

  std::ostringstream text;

  void visitAssignExpr(Assign *expr) override {
    text << "(assign " << expr->name.text << " ";
    accept(expr->value.get());
    text << ')';
  }

  void visitBinaryExpr(Binary *expr) override {
    text << "(" << expr->op.text << ' ';
    accept(expr->left.get());
    text << ' ';
    accept(expr->right.get());
    text << ')';
  }

  void visitCallExpr(Call *expr) override {
    text << "(call ";
    accept(expr->callee.get());
    text << " (";
    for (auto &arg : expr->arguments) {
      accept(arg.get());
      text << ' ';
    }
    text << "))";
  }

  void visitGetExpr(Get *expr) override {
    text << "(get ";
    accept(expr->object.get());
    text << ' ' << expr->name.text << ')';
  }

  void visitGroupingExpr(Grouping *expr) override {
    text << "(grouping ";
    accept(expr->expression.get());
    text << ')';
  }

  void visitLiteralExpr(Literal *expr) override {
    text << "(literal " << expr->value << ')';
  }

  void visitLogicalExpr(Logical *expr) override {
    text << '(' << expr->op.text << ' ';
    accept(expr->left.get());
    text << ' ';
    accept(expr->right.get());
  }

  void visitSetExpr(Set *expr) override {
    text << "(set ";
    accept(expr->object.get());
    text << ' ' << expr->name.text << ' ';
    accept(expr->value.get());
  }

  void visitSuperExpr(Super *expr) override {
    text << "(super " << expr->keyword.text << ' ' << expr->method.text << ')';
  }

  void visitThisExpr(This *expr) override { text << expr->keyword.text; }

  void visitUnaryExpr(Unary *expr) override {
    text << '(' << expr->op.text << ' ';
    accept(expr->right.get());
    text << ')';
  }

  void visitVariableExpr(Variable *expr) override { text << expr->name.text; }

  void visitBlockStmt(Block *stmt) override {
    text << "(block";
    for (const auto &s : stmt->statements) {
      text << ' ';
      accept(s.get());
    }
    text << ')';
  }

  void visitClassStmt(Class *stmt) override {
    text << "(class " << stmt->name.text << ' ';
    accept(stmt->superclass.get());
    text << " (methods";
    for (const auto &m : stmt->methods) {
      text << ' ';
      accept(m.get());
    }
    text << "))";
  }

  void visitExpressionStmt(Expression *stmt) override {
    text << "(expressionStmt ";
    accept(stmt->expression.get());
    text << ')';
  }

  void visitFunctionStmt(Function *stmt) override {
    text << "(funcDef " << stmt->name.text << " (params";
    for (const auto &p : stmt->params) {
      text << ' ' << p.text;
    }
    text << ") (body";
    for (const auto &s : stmt->body) {
      text << ' ';
      accept(s.get());
    }
    text << "))";
  }

  void visitIfStmt(If *stmt) override {
    text << "(if (cond ";
    accept(stmt->condition.get());
    text << ") (then ";
    accept(stmt->thenBranch.get());
    if (stmt->elseBranch.get() != nullptr) {
      text << ") (else ";
      accept(stmt->elseBranch.get());
    }
    text << "))";
  }

  void visitPrintStmt(Print *stmt) override {
    text << "(print ";
    accept(stmt->expression.get());
    text << ')';
  }

  void visitReturnStmt(Return *stmt) override {
    text << "(return ";
    accept(stmt->value.get());
    text << ')';
  }

  void visitVarStmt(Var *stmt) override {
    text << "(var " << stmt->name.text << ' ';
    accept(stmt->initializer.get());
    text << ')';
  }

  void visitWhileStmt(While *stmt) override {
    text << "(while (cond ";
    accept(stmt->condition.get());
    text << ") (body ";
    accept(stmt->body.get());
    text << "))";
  }
};

} // namespace

std::string Ast::stringify() {
  StringifyVisitor v;
  v.accept(this);
  return v.text.str();
}