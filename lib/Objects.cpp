#include "lib/Objects.hpp"
#include <iostream>

using namespace loxlang;

std::ostream &loxlang::operator<<(std::ostream &out, const Value &v) {
  switch (v.type()) {
  case Value::Type::Nil: out << "nil"; break;
  case Value::Type::Pointer: out << v.getObject(); break;
  case Value::Type::Number: out << v.getNumber(); break;
  case Value::Type::String: out << '\"' << v.getString() << '\"'; break;
  case Value::Type::Boolean: out << (v.getBool() ? "true" : "false"); break;
  default: lox_fail("Bad Value Type");
  }
  return out;
}