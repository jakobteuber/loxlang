
#ifndef LOXLANG_LIB_AST_HPP
#define LOXLANG_LIB_AST_HPP

#include "lib/Error.hpp"
#include "lib/Objects.hpp"
#include "lib/Scanner.hpp"
#include <memory>
#include <string>

namespace loxlang::ast {

enum class AstType : std::uint8_t {
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

struct Assign : public Ast {
  Assign(scan::Token name, std::unique_ptr<Ast> value)
      : name{name}, value{std::move(value)} {}
  ~Assign() override = default;
  AstType type() const override { return AstType::AssignExpr; }
  scan::Token name;
  std::unique_ptr<Ast> value;
};

struct Binary : public Ast {
  Binary(std::unique_ptr<Ast> left, scan::Token op, std::unique_ptr<Ast> right)
      : left{std::move(left)}, op{op}, right{std::move(right)} {}
  ~Binary() override = default;
  AstType type() const override { return AstType::BinaryExpr; }
  std::unique_ptr<Ast> left;
  scan::Token op;
  std::unique_ptr<Ast> right;
};

struct Call : public Ast {
  Call(std::unique_ptr<Ast> callee, scan::Token paren,
       std::vector<std::unique_ptr<Ast>> arguments)
      : callee{std::move(callee)}, paren{paren},
        arguments{std::move(arguments)} {}
  ~Call() override = default;
  AstType type() const override { return AstType::CallExpr; }
  std::unique_ptr<Ast> callee;
  scan::Token paren;
  std::vector<std::unique_ptr<Ast>> arguments;
};

struct Get : public Ast {
  Get(std::unique_ptr<Ast> object, scan::Token name)
      : object{std::move(object)}, name{name} {}
  ~Get() override = default;
  AstType type() const override { return AstType::GetExpr; }
  std::unique_ptr<Ast> object;
  scan::Token name;
};

struct Grouping : public Ast {
  explicit Grouping(std::unique_ptr<Ast> expression)
      : expression{std::move(expression)} {}
  ~Grouping() override = default;
  AstType type() const override { return AstType::GroupingExpr; }
  std::unique_ptr<Ast> expression;
};

struct Literal : public Ast {
  explicit Literal(Value value) : value{std::move(value)} {}
  ~Literal() override = default;
  AstType type() const override { return AstType::LiteralExpr; }
  Value value;
};

struct Logical : public Ast {
  Logical(std::unique_ptr<Ast> left, scan::Token op, std::unique_ptr<Ast> right)
      : left{std::move(left)}, op{op}, right{std::move(right)} {}
  ~Logical() override = default;
  AstType type() const override { return AstType::LogicalExpr; }
  std::unique_ptr<Ast> left;
  scan::Token op;
  std::unique_ptr<Ast> right;
};

struct Set : public Ast {
  Set(std::unique_ptr<Ast> object, scan::Token name, std::unique_ptr<Ast> value)
      : object{std::move(object)}, name{name}, value{std::move(value)} {}
  ~Set() override = default;
  AstType type() const override { return AstType::SetExpr; }
  std::unique_ptr<Ast> object;
  scan::Token name;
  std::unique_ptr<Ast> value;
};

struct Super : public Ast {
  Super(scan::Token keyword, scan::Token method)
      : keyword{keyword}, method{method} {}
  ~Super() override = default;
  AstType type() const override { return AstType::SuperExpr; }
  scan::Token keyword;
  scan::Token method;
};

struct This : public Ast {
  explicit This(scan::Token keyword) : keyword{keyword} {}
  ~This() override = default;
  AstType type() const override { return AstType::ThisExpr; }
  scan::Token keyword;
};

struct Unary : public Ast {
  Unary(scan::Token op, std::unique_ptr<Ast> right)
      : op{op}, right{std::move(right)} {}
  ~Unary() override = default;
  AstType type() const override { return AstType::UnaryExpr; }
  scan::Token op;
  std::unique_ptr<Ast> right;
};

struct Variable : public Ast {
  explicit Variable(scan::Token name) : name{name} {}
  ~Variable() override = default;
  AstType type() const override { return AstType::VariableExpr; }
  scan::Token name;
};

struct Block : public Ast {
  explicit Block(std::vector<std::unique_ptr<Ast>> statements)
      : statements{std::move(statements)} {}
  ~Block() override = default;
  AstType type() const override { return AstType::BlockStmt; }
  std::vector<std::unique_ptr<Ast>> statements;
};

struct Expression : public Ast {
  explicit Expression(std::unique_ptr<Ast> expression)
      : expression{std::move(expression)} {}
  ~Expression() override = default;
  AstType type() const override { return AstType::ExpressionStmt; }
  std::unique_ptr<Ast> expression;
};
struct Function : public Ast {
  Function(scan::Token name, std::vector<scan::Token> params,
           std::vector<std::unique_ptr<Ast>> body)
      : name{name}, params{std::move(params)}, body{std::move(body)} {}
  ~Function() override = default;
  AstType type() const override { return AstType::FunctionStmt; }
  scan::Token name;
  std::vector<scan::Token> params;
  std::vector<std::unique_ptr<Ast>> body;
};
struct If : public Ast {
  If(std::unique_ptr<Ast> condition, std::unique_ptr<Ast> thenBranch,
     std::unique_ptr<Ast> elseBranch)
      : condition{std::move(condition)}, thenBranch{std::move(thenBranch)},
        elseBranch{std::move(elseBranch)} {}
  ~If() override = default;
  AstType type() const override { return AstType::IfStmt; }
  std::unique_ptr<Ast> condition;
  std::unique_ptr<Ast> thenBranch;
  std::unique_ptr<Ast> elseBranch;
};
struct Print : public Ast {
  explicit Print(std::unique_ptr<Ast> expression)
      : expression{std::move(expression)} {}
  ~Print() override = default;
  AstType type() const override { return AstType::PrintStmt; }
  std::unique_ptr<Ast> expression;
};
struct Return : public Ast {
  Return(scan::Token keyword, std::unique_ptr<Ast> value)
      : keyword{keyword}, value{std::move(value)} {}
  ~Return() override = default;
  AstType type() const override { return AstType::ReturnStmt; }
  scan::Token keyword;
  std::unique_ptr<Ast> value;
};
struct Var : public Ast {
  Var(scan::Token name, std::unique_ptr<Ast> initializer)
      : name{name}, initializer{std::move(initializer)} {}
  ~Var() override = default;
  AstType type() const override { return AstType::VarStmt; }
  scan::Token name;
  std::unique_ptr<Ast> initializer;
};

struct While : public Ast {
  While(std::unique_ptr<Ast> condition, std::unique_ptr<Ast> body)
      : condition{std::move(condition)}, body{std::move(body)} {}
  ~While() override = default;
  AstType type() const override { return AstType::WhileStmt; }
  std::unique_ptr<Ast> condition;
  std::unique_ptr<Ast> body;
};

struct Class : public Ast {
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
