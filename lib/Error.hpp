#ifndef LOXLANG_LIB_ERROR_HPP
#define LOXLANG_LIB_ERROR_HPP

#include <cstddef>
#include <string_view>

namespace loxlang::error {

void report(std::string_view msg);

void assertError(std::string_view msg, std::string_view expr,
                 std::string_view file, std::size_t line);
void assertError(std::string_view msg, std::string_view left,
                 std::string_view op, std::string_view right,
                 std::string_view file, std::size_t line);

#define lox_assert(cond, msg)                                                  \
  if (!(cond)) {                                                               \
    loxlang::error::assertError(msg, #cond, __FILE__, __LINE__);               \
  }

#define lox_assert_eq(a, b, msg)                                               \
  if ((a) != (b)) {                                                            \
    loxlang::error::assertError(msg, #a, "==", #b, __FILE__, __LINE__);        \
  }

#define lox_assert_neq(a, b, msg)                                              \
  if ((a) == (b)) {                                                            \
    loxlang::error::assertError(msg, #a, "!=", #b, __FILE__, __LINE__);        \
  }

#define lox_fail(msg)                                                          \
  loxlang::error::assertError(msg, "failure", __FILE__, __LINE__)

} // namespace loxlang::error

#endif
