

exprs = [
    "Assign   @ scan::Token name, std::unique_ptr<Expr> value",
    "Binary   @ std::unique_ptr<Expr> left, scan::Token op, std::unique_ptr<Expr> right",
    "Call     @ std::unique_ptr<Expr> callee, scan::Token paren, std::vector<std::unique_ptr<Expr>> arguments",
    "Get      @ std::unique_ptr<Expr> object, scan::Token name",
    "Grouping @ std::unique_ptr<Expr> expression",
    "Literal  @ void* value",
    "Logical  @ std::unique_ptr<Expr> left, scan::Token op, std::unique_ptr<Expr> right",
    "Set      @ std::unique_ptr<Expr> object, scan::Token name, std::unique_ptr<Expr> value",
    "Super    @ scan::Token keyword, scan::Token method",
    "This     @ scan::Token keyword",
    "Unary    @ scan::Token op, std::unique_ptr<Expr> right",
    "Variable @ scan::Token name"
]

statements = [
    "Block      @ std::vector<std::unique_ptr<Stmt>> statements",
    "Class      @ scan::Token name, std::unique_ptr<Variable> superclass, std::vector<std::unique_ptr<Function>> methods",
    "Expression @ std::unique_ptr<Expr> expression",
    "Function   @ scan::Token name, std::vector<scan::Token> params, std::vector<std::unique_ptr<Stmt>> body",
    "If         @ std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch",
    "Print      @ std::unique_ptr<Expr> expression",
    "Return     @ scan::Token keyword, std::unique_ptr<Expr> value",
    "Var        @ scan::Token name, std::unique_ptr<Expr> initializer",
    "While      @ std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body"
]


def className(spec: str): return spec.split("@")[0].strip()
def fields(spec: str): return spec.split("@")[1].split(",")


def fieldNames(spec: str): return map(
    lambda v: v.strip().split(" ")[1], fields(spec))


print("""
#ifndef LOXLANG_LIB_AST_HPP
#define LOXLANG_LIB_AST_HPP

#include "lib/Error.hpp"
#include "lib/Scanner.hpp"
#include <memory>
#include <string>

namespace loxlang::ast {
""")

print("enum AstType {")
for e in exprs:
    print(f"{className(e)}Expr,")
for s in statements:
    print(f"{className(s)}Stmt,")
print("};")

print("std::string astTypeName(AstType type);")


print("""
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
""")

for e in exprs:
    print(f"struct {className(e)};")
for s in statements:
    print(f"struct {className(s)};")


for e in exprs:
    print("struct", className(e), ": public Expr {")
    print(className(e), "(", ",".join(fields(e)), ") : ")
    print(", ".join(n + "{std::move(" + n + ")}" for n in fieldNames(e)))
    print("{ }")
    print(f"~{className(e)}() override = default;")
    print("AstType type() const override ",
          "{ return AstType::", f"{className(e)}Expr", "; }")
    for v in fields(e):
        print(v, ";")
    print("};")

for s in statements:
    print("struct", className(s), ": public Stmt {")
    print(className(s), "(", ",".join(fields(s)), ") : ")
    print(", ".join(n + "{std::move(" + n + ")}" for n in fieldNames(s)))
    print("{ }")
    print(f"~{className(s)}() override = default;")
    print("AstType type() const override ",
          "{ return AstType::", f"{className(s)}Stmt", "; }")
    for v in fields(s):
        print(v, ";")
    print("};")


print("template<typename R> struct Visitor { ")
for e in exprs:
    print(f"virtual R visit{className(e)}Expr({className(e)}* expr) = 0;")
for s in statements:
    print(f"virtual R visit{className(s)}Stmt({className(s)}* stmt) = 0;")


print("""
  R accept(Ast* ast) {
    lox_assert_neq(ast, nullptr, "Ast node should not be nullptr");
    switch (ast->type()) {
""")
for e in exprs:
    print(
        f"case AstType::{className(e)}Expr: return visit{className(e)}Expr(static_cast<{className(e)}*>(ast));")
for s in statements:
    print(
        f"case AstType::{className(s)}Stmt: return visit{className(s)}Stmt(static_cast<{className(s)}*>(ast));")
print("""
    default: lox_fail("bad ast type");
  }
 }
};
""")

print("""
} // namespace loxlang::ast  

#endif    
""")
