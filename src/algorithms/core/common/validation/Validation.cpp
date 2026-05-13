#include "Validation.hpp"

namespace {

bool isDnaChar(char ch) noexcept {
  return ch == 'A' || ch == 'C' || ch == 'G' || ch == 'T';
}

} // namespace

namespace Validation {

bool isDnaString(const std::string &value) noexcept {
  for (char ch : value) {
    if (!isDnaChar(ch)) {
      return false;
    }
  }
  return true;
}

} // namespace  Validation
