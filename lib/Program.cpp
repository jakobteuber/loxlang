#include "lib/Program.hpp"
#include "Program.hpp"
#include "lib/Error.hpp"
#include <algorithm>
#include <format>
#include <print>
#include <span>

namespace {

std::vector<const char *> findLines(std::string_view text) {
  std::vector<const char *> lineLoc;
  lineLoc.push_back(text.begin());
  for (const char &c : text) {
    if (c == '\n') {
      lineLoc.push_back(&c);
    }
  }
  lineLoc.push_back(text.end());

  return lineLoc;
}

struct Line {
  const char *charOffset;
  std::size_t lineNumber;
};

Line findLine(std::span<const char *> lineLoc, const char *offset) {
  auto lower = std::ranges::lower_bound(lineLoc, offset);
  if (lower != lineLoc.begin()) {
    --lower;
  }

  std::ptrdiff_t index = lower - lineLoc.begin();
  lox_assert(index >= 0, "line index is non-negative");
  std::size_t lineNo = static_cast<std::size_t>(index);
  const char *lineOffset = (lineNo < lineLoc.size()) ? lineLoc[lineNo] : 0;
  return Line{lineOffset, lineNo};
}

std::string_view extractLine(std::string_view text,
                             std::span<const char *> lineLoc,
                             std::size_t lineNo) {
  const char *start =
      (lineNo < lineLoc.size()) ? lineLoc[lineNo] : text.begin();
  const char *end =
      (lineNo + 1 < lineLoc.size()) ? lineLoc[lineNo + 1] : text.end();
  if (start >= text.end()) {
    return "End Of File";
  }
  return std::string_view(start, end);
}

void repeatPrint(char c, std::size_t n) {
  for (size_t i = 0; i < n; ++i) {
    std::print("{}", c);
  }
}

} // namespace

void loxlang::Program::error(std::string_view msg, std::size_t charOffset) {
  error(msg, part(charOffset, 1));
}

void loxlang::Program::error(std::string_view msg, std::string_view tokenText) {
  hadErr = true;
  if (lines.empty()) {
    lines = findLines(text);
  }
  Line startLine = findLine(lines, tokenText.begin());
  Line endLine = findLine(lines, tokenText.end());

  std::println("\033[1m{}:{}:\033[0m {}", filename, startLine.lineNumber, msg);
  std::ptrdiff_t startCol = tokenText.begin() - startLine.charOffset;
  std::ptrdiff_t endCol = tokenText.end() - endLine.charOffset;

  constexpr std::ptrdiff_t maxColumnCount = 255;

  if (startLine.lineNumber == endLine.lineNumber) {
    std::string_view startText = extractLine(text, lines, startLine.lineNumber);
    std::println("l. {} | {}", startLine.lineNumber, startText);
    if (startCol > maxColumnCount || endCol > maxColumnCount ||
        (endCol - startCol) > maxColumnCount) {
      std::println("   at columns {}–{}", startCol, endCol);
    } else {
      std::size_t startAlign =
          std::format("l. {} | ", startLine.lineNumber).size();
      repeatPrint(' ', startAlign + startCol);
      repeatPrint('^', endCol - startCol);
      std::println();
    }
  } else {
    std::string_view startText = extractLine(text, lines, startLine.lineNumber);
    std::string_view endText = extractLine(text, lines, endLine.lineNumber);

    std::println("l. {} | {}", startLine.lineNumber, startText);
    if (startCol > maxColumnCount) {
      std::println("   starting at column {}", startCol);
    } else {
      std::size_t startAlign =
          std::format("l. {} | ", startLine.lineNumber).size();
      repeatPrint(' ', startAlign + startCol);
      std::println("^-- starts here");
    }

    std::println("l. {} | {}", endLine.lineNumber, endText);
    if (endCol > maxColumnCount) {
      std::println("   ending at column {}", endCol);
    } else {
      std::size_t startAlign =
          std::format("l. {} | ", endLine.lineNumber).size();
      repeatPrint(' ', startAlign + endCol);
      std::println("^-- ends here");
    }
  }
}
