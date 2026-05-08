#include "KSimilarityService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"

int KSimilarityService::search(const std::string &first,
                               const std::string &second) const {
  auto dnaFirst = DnaSequence(first);
  auto dnaSecond = DnaSequence(second);
  return _solver.findMinimumSwaps(dnaFirst.value(), dnaSecond.value());
}