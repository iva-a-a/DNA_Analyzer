#include "MinWindowService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"

std::string MinWindowService::find(const std::string &text,
                                   const std::string &pattern) const {
  auto dnaText = DnaSequence(text);
  auto dnaPattern = DnaSequence(pattern);
  return _finder.findMinimumWindow(dnaText.value(), dnaPattern.value());
}