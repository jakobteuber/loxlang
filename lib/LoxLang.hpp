#ifndef LOXLANG_LIB_LOXLANG_HPP
#define LOXLANG_LIB_LOXLANG_HPP

#include <string_view>

namespace loxlang {

/**
 * @brief Run a Read-Evaluate-Print loop on the standard input/output.
 */
void runPrompt();

/**
 * @brief Interpret the contents of a file as a Lox program.
 */
void runFile(std::string_view name);

/**
 * @brief Interpret the contents of a string as a Lox program.
 */
void run(std::string_view filename, std::string_view text);

} // namespace loxlang

#endif
