#ifndef LOXLANG_LIB_UTIL_HPP
#define LOXLANG_LIB_UTIL_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace loxlang::util {

std::string enumName(std::string_view names, std::size_t n);

} // namespace loxlang::util

#endif
