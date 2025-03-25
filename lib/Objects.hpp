#ifndef LOXLANG_LIB_OBJECTS_HPP
#define LOXLANG_LIB_OBJECTS_HPP

#include "lib/Error.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <variant>

namespace loxlang {

class Object {};

class Value {
public:
  enum class Type { Nil, Pointer, Number, String, Boolean };

  Value() : v(std::monostate()) {}
  Value(Object *ptr) : v{ptr} {}
  Value(double d) : v{d} {}
  Value(std::string &s) : v{s} {}
  Value(std::string &&s) : v{s} {}
  Value(bool b) : v{b} {}
  Value(std::nullptr_t) : v(std::monostate()) {}

  void operator=(Object *ptr) { v = ptr; }
  void operator=(double n) { v = n; }
  void operator=(std::string &str) { v = str; }
  void operator=(std::string &&str) { v = str; }
  void operator=(bool b) { v = b; }
  void operator=(std::nullptr_t) { v = std::monostate(); }

  Object *getObject() const { return std::get<Object *>(v); }
  double getNumber() const { return std::get<double>(v); }
  const std::string &getString() const { return std::get<std::string>(v); }
  bool getBool() const { return std::get<bool>(v); }

  Type type() const {
    if (std::holds_alternative<std::monostate>(v)) {
      return Type::Nil;
    }
    if (std::holds_alternative<Object *>(v)) {
      return Type::Pointer;
    }
    if (std::holds_alternative<double>(v)) {
      return Type::Number;
    }
    if (std::holds_alternative<std::string>(v)) {
      return Type::String;
    }
    if (std::holds_alternative<bool>(v)) {
      return Type::Boolean;
    }
    lox_fail("Bad variant type");
  }

private:
  using BackingVariant =
      std::variant<std::monostate, Object *, double, std::string, bool>;
  BackingVariant v;
};

std::ostream &operator<<(std::ostream &out, const Value &v);

} // namespace loxlang

#endif