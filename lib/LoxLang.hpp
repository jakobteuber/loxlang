#pragma once

#include <string_view>

namespace loxlang {

void runPrompt();
void runFile(std::string_view name);
void run(std::string_view filename, std::string_view name);

} // namespace loxlang
