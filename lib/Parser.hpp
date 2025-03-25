#ifndef LOXLANG_LIB_PARSER_HPP
#define LOXLANG_LIB_PARSER_HPP

#include "lib/Program.hpp"
#include "lib/Scanner.hpp"
#include <memory>

namespace loxlang::ast {
struct Ast;
} // namespace loxlang::ast

namespace loxlang::parse {

/**
 * @brief Lox parser
 * @details
 * @param p The program to parse
 * @param s A Scanner for the program
 * @return A Abstract Syntax Tree, or nullptr if an error occurred.
 */
std::unique_ptr<loxlang::ast::Ast> parse(Program &p, scan::Scanner &s);

} // namespace loxlang::parse

#endif
