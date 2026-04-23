#include "Validation.hpp"

bool Validation::IsDnaChar(char ch) noexcept {
  return ch == 'A' || ch == 'C' || ch == 'G' || ch == 'T';
}

bool Validation::IsDnaString(const std::string &value) noexcept {
  for (char ch : value) {
    if (!Validation::IsDnaChar(ch)) {
      return false;
    }
  }
  return true;
}