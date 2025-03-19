#include "lib/LoxLang.hpp"
#include <print>

int main(int argc, char const *argv[]) {
  if (argc == 1) {
    loxlang::runPrompt();
  } else if (argc == 2) {
    loxlang::runFile(argv[1]);
  } else {
    std::println("Usage: {}             -- start a interactive shell", argv[0]);
    std::println("       {} <script>    -- execute a script file", argv[0]);
  }
}
