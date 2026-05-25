#include "Validation.hpp"

namespace Validation {

bool isDnaChar(char ch) noexcept {
  return ch == 'A' || ch == 'C' || ch == 'G' || ch == 'T';
}

bool isDnaString(const std::string &value) noexcept {
  for (char ch : value) {
    if (!isDnaChar(ch)) {
      return false;
    }
  }
  return true;
}

} // namespace  Validation
