#ifndef LOXLANG_LIB_PROGRAM_HPP
#define LOXLANG_LIB_PROGRAM_HPP

#include <cstdint>
#include <string_view>
#include <vector>

namespace loxlang {

/**
 * @brief A class representing the program text
 * @details This class mostly contains routines for error reporting
 */
struct Program {
  Program(std::string_view filename, std::string_view text)
      : filename{filename}, text{text}, lines{} {}

  /**
   * @brief Report an error at a given character offset.
   * @param msg the error message to display to the user
   * @param charOffset the location at which the error occurred in the program
   * text.
   */
  void error(std::string_view msg, std::size_t charOffset);

  /**
   * @brief Report an error at a given program location.
   * @param msg the error message to display to the user
   * @param tokenText a view into the program text representing the program
   * location
   */
  void error(std::string_view msg, std::string_view tokenText);

  /**
   * @brief Getter for the program text
   * @return the verbitem program text
   */
  std::string_view programText() const { return text; }

  /**
   * @brief a window into the program text
   */
  std::string_view part(std::size_t start, std::size_t length) const {
    return text.substr(start, length);
  }

  /**
   * @brief Queries wether an error has already been reported on this program.
   * @return `true`, if the program had an error reported.
   */
  bool hadError() const { return hadErr; }

private:
  std::string_view filename;
  std::string_view text;
  std::vector<const char *> lines;
  bool hadErr = false;
};

} // namespace loxlang

#endif
