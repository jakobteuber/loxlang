
#ifndef LOXLANG_LIB_AST_HPP
#define LOXLANG_LIB_AST_HPP

#include "lib/Error.hpp"
#include "lib/Scanner.hpp"
#include <memory>
#include <string>

namespace loxlang::ast {

enum class AstType {
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
  virtual AstType type() const override = 0;
  virtual ~Expr() override = 0;
};

struct Stmt : public Ast {
  virtual AstType type() const override = 0;
  virtual ~Stmt() override = 0;
};

struct Assign : public Expr {
  Assign(scan::Token name, std::unique_ptr<Expr> value)
      : name{name}, value{std::move(value)} {}
  ~Assign() override = default;
  AstType type() const override { return AstType::AssignExpr; }
  scan::Token name;
  std::unique_ptr<Expr> value;
};

struct Binary : public Expr {
  Binary(std::unique_ptr<Expr> left, scan::Token op,
         std::unique_ptr<Expr> right)
      : left{std::move(left)}, op{op}, right{std::move(right)} {}
  ~Binary() override = default;
  AstType type() const override { return AstType::BinaryExpr; }
  std::unique_ptr<Expr> left;
  scan::Token op;
  std::unique_ptr<Expr> right;
};

struct Call : public Expr {
  Call(std::unique_ptr<Expr> callee, scan::Token paren,
       std::vector<std::unique_ptr<Expr>> arguments)
      : callee{std::move(callee)}, paren{paren},
        arguments{std::move(arguments)} {}
  ~Call() override = default;
  AstType type() const override { return AstType::CallExpr; }
  std::unique_ptr<Expr> callee;
  scan::Token paren;
  std::vector<std::unique_ptr<Expr>> arguments;
};

struct Get : public Expr {
  Get(std::unique_ptr<Expr> object, scan::Token name)
      : object{std::move(object)}, name{name} {}
  ~Get() override = default;
  AstType type() const override { return AstType::GetExpr; }
  std::unique_ptr<Expr> object;
  scan::Token name;
};

struct Grouping : public Expr {
  Grouping(std::unique_ptr<Expr> expression)
      : expression{std::move(expression)} {}
  ~Grouping() override = default;
  AstType type() const override { return AstType::GroupingExpr; }
  std::unique_ptr<Expr> expression;
};

struct Literal : public Expr {
  Literal(void *value) : value{value} {}
  ~Literal() override = default;
  AstType type() const override { return AstType::LiteralExpr; }
  void *value;
};

struct Logical : public Expr {
  Logical(std::unique_ptr<Expr> left, scan::Token op,
          std::unique_ptr<Expr> right)
      : left{std::move(left)}, op{op}, right{std::move(right)} {}
  ~Logical() override = default;
  AstType type() const override { return AstType::LogicalExpr; }
  std::unique_ptr<Expr> left;
  scan::Token op;
  std::unique_ptr<Expr> right;
};

struct Set : public Expr {
  Set(std::unique_ptr<Expr> object, scan::Token name,
      std::unique_ptr<Expr> value)
      : object{std::move(object)}, name{name}, value{std::move(value)} {}
  ~Set() override = default;
  AstType type() const override { return AstType::SetExpr; }
  std::unique_ptr<Expr> object;
  scan::Token name;
  std::unique_ptr<Expr> value;
};

struct Super : public Expr {
  Super(scan::Token keyword, scan::Token method)
      : keyword{keyword}, method{method} {}
  ~Super() override = default;
  AstType type() const override { return AstType::SuperExpr; }
  scan::Token keyword;
  scan::Token method;
};

struct This : public Expr {
  This(scan::Token keyword) : keyword{keyword} {}
  ~This() override = default;
  AstType type() const override { return AstType::ThisExpr; }
  scan::Token keyword;
};

struct Unary : public Expr {
  Unary(scan::Token op, std::unique_ptr<Expr> right)
      : op{op}, right{std::move(right)} {}
  ~Unary() override = default;
  AstType type() const override { return AstType::UnaryExpr; }
  scan::Token op;
  std::unique_ptr<Expr> right;
};

struct Variable : public Expr {
  Variable(scan::Token name) : name{name} {}
  ~Variable() override = default;
  AstType type() const override { return AstType::VariableExpr; }
  scan::Token name;
};

struct Block : public Stmt {
  Block(std::vector<std::unique_ptr<Stmt>> statements)
      : statements{std::move(statements)} {}
  ~Block() override = default;
  AstType type() const override { return AstType::BlockStmt; }
  std::vector<std::unique_ptr<Stmt>> statements;
};

struct Expression : public Stmt {
  Expression(std::unique_ptr<Expr> expression)
      : expression{std::move(expression)} {}
  ~Expression() override = default;
  AstType type() const override { return AstType::ExpressionStmt; }
  std::unique_ptr<Expr> expression;
};
struct Function : public Stmt {
  Function(scan::Token name, std::vector<scan::Token> params,
           std::vector<std::unique_ptr<Stmt>> body)
      : name{name}, params{params}, body{std::move(body)} {}
  ~Function() override = default;
  AstType type() const override { return AstType::FunctionStmt; }
  scan::Token name;
  std::vector<scan::Token> params;
  std::vector<std::unique_ptr<Stmt>> body;
};
struct If : public Stmt {
  If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch,
     std::unique_ptr<Stmt> elseBranch)
      : condition{std::move(condition)}, thenBranch{std::move(thenBranch)},
        elseBranch{std::move(elseBranch)} {}
  ~If() override = default;
  AstType type() const override { return AstType::IfStmt; }
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> thenBranch;
  std::unique_ptr<Stmt> elseBranch;
};
struct Print : public Stmt {
  Print(std::unique_ptr<Expr> expression) : expression{std::move(expression)} {}
  ~Print() override = default;
  AstType type() const override { return AstType::PrintStmt; }
  std::unique_ptr<Expr> expression;
};
struct Return : public Stmt {
  Return(scan::Token keyword, std::unique_ptr<Expr> value)
      : keyword{keyword}, value{std::move(value)} {}
  ~Return() override = default;
  AstType type() const override { return AstType::ReturnStmt; }
  scan::Token keyword;
  std::unique_ptr<Expr> value;
};
struct Var : public Stmt {
  Var(scan::Token name, std::unique_ptr<Expr> initializer)
      : name{name}, initializer{std::move(initializer)} {}
  ~Var() override = default;
  AstType type() const override { return AstType::VarStmt; }
  scan::Token name;
  std::unique_ptr<Expr> initializer;
};

struct While : public Stmt {
  While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
      : condition{std::move(condition)}, body{std::move(body)} {}
  ~While() override = default;
  AstType type() const override { return AstType::WhileStmt; }
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> body;
};

struct Class : public Stmt {
  Class(scan::Token name, std::unique_ptr<Variable> superclass,
        std::vector<std::unique_ptr<Function>> methods)
      : name{name}, superclass{std::move(superclass)},
        methods{std::move(methods)} {}
  ~Class() override = default;
  AstType type() const override { return AstType::ClassStmt; }
  scan::Token name;
  std::unique_ptr<Variable> superclass;
  std::vector<std::unique_ptr<Function>> methods;
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
    lox_assert_neq(ast, nullptr, "Ast node should not be nullptr");
    switch (ast->type()) {

    case AstType::AssignExpr:
      return visitAssignExpr(static_cast<Assign *>(ast));
    case AstType::BinaryExpr:
      return visitBinaryExpr(static_cast<Binary *>(ast));
    case AstType::CallExpr: return visitCallExpr(static_cast<Call *>(ast));
    case AstType::GetExpr: return visitGetExpr(static_cast<Get *>(ast));
    case AstType::GroupingExpr:
      return visitGroupingExpr(static_cast<Grouping *>(ast));
    case AstType::LiteralExpr:
      return visitLiteralExpr(static_cast<Literal *>(ast));
    case AstType::LogicalExpr:
      return visitLogicalExpr(static_cast<Logical *>(ast));
    case AstType::SetExpr: return visitSetExpr(static_cast<Set *>(ast));
    case AstType::SuperExpr: return visitSuperExpr(static_cast<Super *>(ast));
    case AstType::ThisExpr: return visitThisExpr(static_cast<This *>(ast));
    case AstType::UnaryExpr: return visitUnaryExpr(static_cast<Unary *>(ast));
    case AstType::VariableExpr:
      return visitVariableExpr(static_cast<Variable *>(ast));
    case AstType::BlockStmt: return visitBlockStmt(static_cast<Block *>(ast));
    case AstType::ClassStmt: return visitClassStmt(static_cast<Class *>(ast));
    case AstType::ExpressionStmt:
      return visitExpressionStmt(static_cast<Expression *>(ast));
    case AstType::FunctionStmt:
      return visitFunctionStmt(static_cast<Function *>(ast));
    case AstType::IfStmt: return visitIfStmt(static_cast<If *>(ast));
    case AstType::PrintStmt: return visitPrintStmt(static_cast<Print *>(ast));
    case AstType::ReturnStmt:
      return visitReturnStmt(static_cast<Return *>(ast));
    case AstType::VarStmt: return visitVarStmt(static_cast<Var *>(ast));
    case AstType::WhileStmt: return visitWhileStmt(static_cast<While *>(ast));

    default: lox_fail("bad ast type");
    }
  }
};

} // namespace loxlang::ast

#endif
