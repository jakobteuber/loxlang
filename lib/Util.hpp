#ifndef LOXLANG_LIB_UTIL_HPP
#define LOXLANG_LIB_UTIL_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace loxlang::util {

/**
 * @brief Return a string representation of an enum constant.
 * @details This is strictly for debug purposes and is implemented quite
 * inefficiently and dumb.
 *
 * ```cpp
 * enum MyEnum {a, b, c};
 * enumName("a, b, c", 1) // → returns "b"
 * ```
 *
 * @param names The names of the enum constants, separated by commas.
 * @param n The integral constant that represents the enum value.
 * @return The string representation of the enum.
 */
std::string enumName(std::string_view names, std::size_t n);

} // namespace loxlang::util

#endif
