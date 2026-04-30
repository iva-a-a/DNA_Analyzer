#include "Validation.hpp"

bool Validation::isDnaChar(char ch) noexcept {
  return ch == 'A' || ch == 'C' || ch == 'G' || ch == 'T';
}

bool Validation::isDnaString(const std::string &value) noexcept {
  for (char ch : value) {
    if (!Validation::isDnaChar(ch)) {
      return false;
    }
  }
  return true;
}