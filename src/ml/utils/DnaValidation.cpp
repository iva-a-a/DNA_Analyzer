#include "DnaValidation.hpp"

namespace {

bool isDnaChar(char ch) noexcept {
  return ch == 'A' || ch == 'C' || ch == 'G' || ch == 'T' || ch == 'N';
}

} // namespace

namespace DnaValidation {

bool isDnaSequence(const std::string &sequence) {
  for (char ch : sequence) {
    if (!isDnaChar(ch)) {
      return false;
    }
  }
  return true;
}

} // namespace DnaValidation