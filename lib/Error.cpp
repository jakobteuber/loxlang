#include "lib/Error.hpp"
#include <boost/stacktrace.hpp>
#include <print>
#include <sstream>

namespace loxlang::error {

void report(std::string_view msg) { std::println("{}", msg); }

void assertError(std::string_view msg, std::string_view file,
                 std::string_view line) {
  std::println("\033[1;31m{}{}:\033[0m\033[31m", file, line);
  std::println("{}", msg);

  auto trace = boost::stacktrace::stacktrace();
  auto asText = std::ostringstream();
  asText << trace;
  std::print("{}", asText.str());
  std::println("\033[0m");
}

} // namespace loxlang::error
