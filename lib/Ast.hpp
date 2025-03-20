
#pragma once

#include "lib/Error.hpp"
#include "lib/Scanner.hpp"
#include <memory>
#include <string>

namespace loxlang::ast {

enum AstType {
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
};
std::string astTypeName(AstType type);

struct Ast {
  virtual AstType type() const = 0;
  virtual ~Ast() = 0;
  std::string stringify();
};
struct Expr : public Ast {
  virtual AstType type() const = 0;
  virtual ~Expr() = 0;
};
struct Stmt : public Ast {
  virtual AstType type() const override = 0;
  virtual ~Stmt() override = 0;
};

struct Assign : public Expr {
  ~Assign() override = default;
  AstType type() const override { return AstType::AssignExpr; }
  scan::Token name;
  std::unique_ptr<Expr> value;
};
struct Binary : public Expr {
  ~Binary() override = default;
  AstType type() const override { return AstType::BinaryExpr; }
  std::unique_ptr<Expr> left;
  scan::Token op;
  std::unique_ptr<Expr> right;
};
struct Call : public Expr {
  ~Call() override = default;
  AstType type() const override { return AstType::CallExpr; }
  std::unique_ptr<Expr> callee;
  scan::Token paren;
  std::vector<std::unique_ptr<Expr>> arguments;
};
struct Get : public Expr {
  ~Get() override = default;
  AstType type() const override { return AstType::GetExpr; }
  std::unique_ptr<Expr> object;
  scan::Token name;
};
struct Grouping : public Expr {
  ~Grouping() override = default;
  AstType type() const override { return AstType::GroupingExpr; }
  std::unique_ptr<Expr> expression;
};
struct Literal : public Expr {
  ~Literal() override = default;
  AstType type() const override { return AstType::LiteralExpr; }
  void *value;
};
struct Logical : public Expr {
  ~Logical() override = default;
  AstType type() const override { return AstType::LogicalExpr; }
  std::unique_ptr<Expr> left;
  scan::Token op;
  std::unique_ptr<Expr> right;
};
struct Set : public Expr {
  ~Set() override = default;
  AstType type() const override { return AstType::SetExpr; }
  std::unique_ptr<Expr> object;
  scan::Token name;
  std::unique_ptr<Expr> value;
};
struct Super : public Expr {
  ~Super() override = default;
  AstType type() const override { return AstType::SuperExpr; }
  scan::Token keyword;
  scan::Token method;
};
struct This : public Expr {
  ~This() override = default;
  AstType type() const override { return AstType::ThisExpr; }
  scan::Token keyword;
};
struct Unary : public Expr {
  ~Unary() override = default;
  AstType type() const override { return AstType::UnaryExpr; }
  scan::Token op;
  scan::Token right;
};
struct Variable : public Expr {
  ~Variable() override = default;
  AstType type() const override { return AstType::VariableExpr; }
  scan::Token name;
};
struct Block : public Stmt {
  ~Block() override = default;
  AstType type() const override { return AstType::BlockStmt; }
  scan::Token name;
};
struct Class : public Stmt {
  ~Class() override = default;
  AstType type() const override { return AstType::ClassStmt; }
  scan::Token name;
};
struct Expression : public Stmt {
  ~Expression() override = default;
  AstType type() const override { return AstType::ExpressionStmt; }
  scan::Token name;
};
struct Function : public Stmt {
  ~Function() override = default;
  AstType type() const override { return AstType::FunctionStmt; }
  scan::Token name;
};
struct If : public Stmt {
  ~If() override = default;
  AstType type() const override { return AstType::IfStmt; }
  scan::Token name;
};
struct Print : public Stmt {
  ~Print() override = default;
  AstType type() const override { return AstType::PrintStmt; }
  scan::Token name;
};
struct Return : public Stmt {
  ~Return() override = default;
  AstType type() const override { return AstType::ReturnStmt; }
  scan::Token name;
};
struct Var : public Stmt {
  ~Var() override = default;
  AstType type() const override { return AstType::VarStmt; }
  scan::Token name;
};
struct While : public Stmt {
  ~While() override = default;
  AstType type() const override { return AstType::WhileStmt; }
  scan::Token name;
};
template <typename R> struct Visitor {
  virtual R visitAssignExpr(Assign *expr) = 0;
  virtual R visitBinaryExpr(Binary *expr) = 0;
  virtual R visitCallExpr(Call *expr) = 0;
  virtual R visitGetExpr(Get *expr) = 0;
  virtual R visitGroupingExpr(Grouping *expr) = 0;
  virtual R visitLiteralExpr(Literal *expr) = 0;
  virtual R visitLogicalExpr(Logical *expr) = 0;
  virtual R visitSetExpr(Set *expr) = 0;
  virtual R visitSuperExpr(Super *expr) = 0;
  virtual R visitThisExpr(This *expr) = 0;
  virtual R visitUnaryExpr(Unary *expr) = 0;
  virtual R visitVariableExpr(Variable *expr) = 0;
  virtual R visitBlockStmt(Block *stmt) = 0;
  virtual R visitClassStmt(Class *stmt) = 0;
  virtual R visitExpressionStmt(Expression *stmt) = 0;
  virtual R visitFunctionStmt(Function *stmt) = 0;
  virtual R visitIfStmt(If *stmt) = 0;
  virtual R visitPrintStmt(Print *stmt) = 0;
  virtual R visitReturnStmt(Return *stmt) = 0;
  virtual R visitVarStmt(Var *stmt) = 0;
  virtual R visitWhileStmt(While *stmt) = 0;

  R accept(Ast *ast) {
    switch (ast->type()) {

    case AstType::AssignExpr:
      return visitAssignExpr(static_cast<Assign *>(this));
    case AstType::BinaryExpr:
      return visitBinaryExpr(static_cast<Binary *>(this));
    case AstType::CallExpr: return visitCallExpr(static_cast<Call *>(this));
    case AstType::GetExpr: return visitGetExpr(static_cast<Get *>(this));
    case AstType::GroupingExpr:
      return visitGroupingExpr(static_cast<Grouping *>(this));
    case AstType::LiteralExpr:
      return visitLiteralExpr(static_cast<Literal *>(this));
    case AstType::LogicalExpr:
      return visitLogicalExpr(static_cast<Logical *>(this));
    case AstType::SetExpr: return visitSetExpr(static_cast<Set *>(this));
    case AstType::SuperExpr: return visitSuperExpr(static_cast<Super *>(this));
    case AstType::ThisExpr: return visitThisExpr(static_cast<This *>(this));
    case AstType::UnaryExpr: return visitUnaryExpr(static_cast<Unary *>(this));
    case AstType::VariableExpr:
      return visitVariableExpr(static_cast<Variable *>(this));
    case AstType::BlockStmt: return visitBlockStmt(static_cast<Block *>(this));
    case AstType::ClassStmt: return visitClassStmt(static_cast<Class *>(this));
    case AstType::ExpressionStmt:
      return visitExpressionStmt(static_cast<Expression *>(this));
    case AstType::FunctionStmt:
      return visitFunctionStmt(static_cast<Function *>(this));
    case AstType::IfStmt: return visitIfStmt(static_cast<If *>(this));
    case AstType::PrintStmt: return visitPrintStmt(static_cast<Print *>(this));
    case AstType::ReturnStmt:
      return visitReturnStmt(static_cast<Return *>(this));
    case AstType::VarStmt: return visitVarStmt(static_cast<Var *>(this));
    case AstType::WhileStmt: return visitWhileStmt(static_cast<While *>(this));

    default: lox_fail("bad ast type");
    }
  }
};

} // namespace loxlang::ast
