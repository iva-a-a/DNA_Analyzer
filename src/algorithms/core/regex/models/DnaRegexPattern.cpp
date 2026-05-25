#include "DnaRegexPattern.hpp"

#include "../../common/errors/Errors.hpp"
#include "../../common/validation/Validation.hpp"

namespace {

bool isRegexOperator(char c) noexcept {
  return c == '.' || c == '*' || c == '?' || c == '+';
}

bool isAllowedDnaRegexChar(char c) noexcept {
  return Validation::isDnaChar(c) || isRegexOperator(c);
}

} // namespace

DnaRegexPattern::DnaRegexPattern(const std::string &value) {
  if (value.empty()) {
    throw ValidationError("DNA regex pattern cannot be empty");
  }

  for (char c : value) {
    if (!isAllowedDnaRegexChar(c)) {
      throw ValidationError(
          "DNA regex pattern can contain only DNA symbols and regex operators");
    }
  }

  _value = value;
}

const std::string &DnaRegexPattern::value() const noexcept { return _value; }