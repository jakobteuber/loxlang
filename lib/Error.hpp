#pragma once

#include <string_view>

namespace loxlang::error {

void report(std::string_view msg);

void assertError(std::string_view msg, std::string_view file,
                 std::string_view line);

} // namespace loxlang::error
