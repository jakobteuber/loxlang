#include "lib/Util.hpp"
#include "lib/Error.hpp"
#include <regex>
#include <vector>

std::string loxlang::util::enumName(std::string_view names, std::size_t n) {
  std::regex item = std::regex(R"REGEX(\s*(\w+)\s*,\s*)REGEX");
  std::string owned = std::string(names);
  auto begin = std::sregex_iterator(owned.begin(), owned.end(), item);
  auto end = std::sregex_iterator();
  auto iter = begin;
  for (std::size_t count = 0; count != n; ++count) {
    lox_assert_neq(iter, end, "too few enum names for count");
    iter++;
  }
  std::smatch match = *iter;
  return match[1].str();
}