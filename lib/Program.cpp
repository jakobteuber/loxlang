#include "lib/Program.hpp"
#include "Program.hpp"
#include "lib/Error.hpp"
#include <algorithm>
#include <format>
#include <print>
#include <span>

namespace {

std::vector<std::size_t> findLines(std::string_view text) {
  std::size_t offset = 0;
  std::vector<std::size_t> lineLoc;
  lineLoc.push_back(0);
  for (char c : text) {
    if (c == '\n') {
      lineLoc.push_back(offset);
    }
    offset++;
  }
  lineLoc.push_back(std::string_view::npos);

  return lineLoc;
}

struct Line {
  std::size_t charOffset;
  std::size_t lineNumber;
};

Line findLine(std::span<std::size_t> lineLoc, std::size_t offset) {
  auto lower = std::ranges::lower_bound(lineLoc, offset);
  if (lower != lineLoc.begin()) {
    --lower;
  }

  std::ptrdiff_t index = lower - lineLoc.begin();
  lox_assert(index >= 0, "line index is non-negative");
  std::size_t lineNo = static_cast<std::size_t>(index);
  std::size_t lineOffset = (lineNo < lineLoc.size()) ? lineLoc[lineNo] : 0;
  return Line{lineOffset, lineNo};
}

std::string_view extractLine(std::string_view text,
                             std::span<std::size_t> lineLoc,
                             std::size_t lineNo) {
  std::size_t start = (lineNo < lineLoc.size()) ? lineLoc[lineNo] : 0;
  std::size_t end = (lineNo + 1 < lineLoc.size()) ? lineLoc[lineNo + 1]
                                                  : std::string_view::npos;
  if (start >= text.size()) {
    return "End Of File";
  }
  return text.substr(start, end - start);
}

void repeatPrint(char c, std::size_t n) {
  for (size_t i = 0; i < n; ++i) {
    std::print("{}", c);
  }
}

} // namespace

void loxlang::Program::error(std::string_view msg, std::size_t charOffset) {
  error(msg, charOffset, charOffset + 1);
}

void loxlang::Program::error(std::string_view msg, std::size_t startOffset,
                             std::size_t endOffset) {
  if (lines.empty()) {
    lines = findLines(text);
  }
  Line startLine = findLine(lines, startOffset);
  Line endLine = findLine(lines, endOffset);

  std::println("\033[1m{}:{}:\033[0m {}", filename, startLine.lineNumber, msg);
  std::size_t startCol = startOffset - startLine.charOffset;
  std::size_t endCol = endOffset - endLine.charOffset;

  constexpr std::size_t maxColumnCount = 255;

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
