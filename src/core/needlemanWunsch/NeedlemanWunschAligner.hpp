#pragma once

#include "model/AlignmentResult.hpp"
#include "model/ScoringScheme.hpp"
#include <vector>

class NeedlemanWunschAligner {
public:
  int computeScore(const std::string &first, const std::string &second,
                   const ScoringScheme &scoring) const;
  AlignmentResult align(const std::string &first, const std::string &second,
                        const ScoringScheme &scoring) const;

private:
  int scorePair(char lhs, char rhs, const ScoringScheme &scoring) const;
  std::vector<std::vector<int>>
  buildScoreMatrix(const std::string &first, const std::string &second,
                   const ScoringScheme &scoring) const;
  char buildMatchChar(char lhs, char rhs) const;
  AlignmentResult
  traceback(const std::string &first, const std::string &second,
            const ScoringScheme &scoring,
            const std::vector<std::vector<int>> &scoreMatrix) const;
};
