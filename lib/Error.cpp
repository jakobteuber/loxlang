#include "lib/Error.hpp"
#include <boost/stacktrace.hpp>
#include <exception>
#include <print>
#include <sstream>

namespace {

void printStackTrace() {
  auto trace = boost::stacktrace::stacktrace();
  auto asText = std::ostringstream();
  asText << trace;
  std::print("{}", asText.str());
}

} // namespace

[[noreturn]] void loxlang::error::assertError(std::string_view msg,
                                              std::string_view expr,
                                              std::string_view file,
                                              std::size_t line) {
  std::println("\033[1;31m{}:{}:\033[0m\033[31m", file, line);
  std::println("{}", msg);
  std::println("\t{}", expr);
  printStackTrace();
  std::println("\033[0m");
  std::terminate();
}
