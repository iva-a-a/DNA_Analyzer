#pragma once

#include "../../core/needlemanWunsch/NeedlemanWunschAligner.hpp"

class AlignmentService {
public:
  AlignmentService() = default;

  int computeScore(const std::string &first, const std::string &second,
                   const ScoringScheme &scoring) const;
  AlignmentResult align(const std::string &first, const std::string &second,
                        const ScoringScheme &scoring) const;

private:
  NeedlemanWunschAligner _aligner;
};