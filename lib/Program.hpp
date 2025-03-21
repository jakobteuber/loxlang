#ifndef LOXLANG_LIB_PROGRAM_HPP
#define LOXLANG_LIB_PROGRAM_HPP

#include <cstdint>
#include <string_view>
#include <vector>

namespace loxlang {

struct Program {
  Program(std::string_view filename, std::string_view text)
      : filename{filename}, text{text}, lines{} {}
  void error(std::string_view msg, std::size_t charOffset);
  void error(std::string_view msg, std::string_view tokenText);

  std::string_view programText() const { return text; }
  std::string_view part(std::size_t start, std::size_t length) const {
    return text.substr(start, length);
  }

  bool hadError() const { return hadErr; }

private:
  std::string_view filename;
  std::string_view text;
  std::vector<const char *> lines;
  bool hadErr = false;
};

} // namespace loxlang

#endif
