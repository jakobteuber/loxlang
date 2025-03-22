/**
 * @file
 * @brief Primary interaction point with Loxlang.
 * @details
 * If you don’t need deep control of the Loxlang interna, this is the only
 * header that you will need. The `loxlang` program is just a thin wrapper that
 * around this header provides.
 */

#ifndef LOXLANG_LIB_LOXLANG_HPP
#define LOXLANG_LIB_LOXLANG_HPP

#include <string_view>

/**
 * @namespace loxlang
 * The namespace containing all Loxlang functionality.
 */
namespace loxlang {

/**
 * @brief Run a Read-Evaluate-Print loop on the standard input/output.
 * @details This is roughly equivalent to run `loxlang` without any additional
 * parameters to enter the REPL.
 */
void runPrompt();

/**
 * @brief Interpret the contents of a file as a Lox program.
 * @details This is roughly equivalent to run `loxlang` with a file argument.
 * @param name the name of the file
 */
void runFile(std::string_view name);

/**
 * @brief Interpret the contents of a string as a Lox program.
 * @param filename The name that the interpreter will use when telling the user
 * about errors and problems. It does not have to be an actual file name, it can
 * be an arbitrary string that will be helpfull to the user. The
 * Read-Evaluate-Print Loop will pass "REPL" here.
 * @param text The program text to interpret.
 */
void run(std::string_view filename, std::string_view text);

} // namespace loxlang

#endif
