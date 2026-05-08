#include "AlignmentService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"

int AlignmentService::computeScore(const std::string &first,
                                   const std::string &second,
                                   const ScoringScheme &scoring) const {

  auto dnaFirst = DnaSequence(first);
  auto dnaSecond = DnaSequence(second);

  return _aligner.computeScore(dnaFirst.value(), dnaSecond.value(), scoring);
}

AlignmentResult AlignmentService::align(const std::string &first,
                                        const std::string &second,
                                        const ScoringScheme &scoring) const {

  auto dnaFirst = DnaSequence(first);
  auto dnaSecond = DnaSequence(second);

  return _aligner.align(dnaFirst.value(), dnaSecond.value(), scoring);
}