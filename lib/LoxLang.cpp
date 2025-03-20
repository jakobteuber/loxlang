#include "lib/LoxLang.hpp"
#include "lib/Program.hpp"
#include "lib/Scanner.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <string>

namespace {

std::string readFile(std::string_view name) {
  using namespace std::filesystem;

  std::println("run file {}...", name);
  path path = absolute(name);

  if (!exists(path)) {
    std::println("file {} does not exist", path.c_str());
    return "";
  }

  std::ifstream in{path};
  auto begin = std::istreambuf_iterator<char>(in);
  auto end = std::istreambuf_iterator<char>();
  return std::string(begin, end);
}

std::string readFromPrompt() {
  std::print("\033[1mlox>\033[0m ");
  std::string line;
  std::getline(std::cin, line);
  return line;
}

} // namespace

void loxlang::runPrompt() {
  while (true) {
    std::string text = readFromPrompt();
    if (text.empty()) {
      break;
    }
    run("REPL", text);
  }
}

void loxlang::runFile(std::string_view name) {
  std::string text = readFile(name);
  run(name, text);
}

void loxlang::run(std::string_view filename, std::string_view text) {
  using namespace scan;

  if (text.empty()) {
    return;
  }

  auto program = Program(filename, text);
  Scanner scanner = Scanner(program);
  for (Token t = scanner.next(); t.type != Token::Type::Eof;
       t = scanner.next()) {
    std::println("Token({}, {}, {})", Token::typeName(t.type), t.text,
                 t.text.size());
  }
}
