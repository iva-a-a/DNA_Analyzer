#include "NeedlemanWunschAligner.hpp"
#include <algorithm>

int NeedlemanWunschAligner::computeScore(const std::string &first,
                                         const std::string &second,
                                         const ScoringScheme &scoring) const {
  std::vector<std::vector<int>> scoreMatrix =
      buildScoreMatrix(first, second, scoring);
  return scoreMatrix[first.size()][second.size()];
}

AlignmentResult
NeedlemanWunschAligner::align(const std::string &first,
                              const std::string &second,
                              const ScoringScheme &scoring) const {
  std::vector<std::vector<int>> scoreMatrix =
      buildScoreMatrix(first, second, scoring);

  return traceback(first, second, scoring, scoreMatrix);
}

int NeedlemanWunschAligner::scorePair(char lhs, char rhs,
                                      const ScoringScheme &scoring) const {
  return lhs == rhs ? scoring.matchScore : scoring.mismatchScore;
}

std::vector<std::vector<int>>
NeedlemanWunschAligner::buildScoreMatrix(const std::string &first,
                                         const std::string &second,
                                         const ScoringScheme &scoring) const {
  std::size_t n = first.size();
  std::size_t m = second.size();
  std::vector<std::vector<int>> scoreMatrix(n + 1, std::vector<int>(m + 1, 0));

  for (std::size_t i = 0; i <= n; ++i) {
    scoreMatrix[i][0] = static_cast<int>(i) * scoring.gapScore;
  }
  for (std::size_t j = 0; j <= m; ++j) {
    scoreMatrix[0][j] = static_cast<int>(j) * scoring.gapScore;
  }
  for (std::size_t i = 1; i <= n; ++i) {
    for (std::size_t j = 1; j <= m; ++j) {
      int diagonal = scoreMatrix[i - 1][j - 1] +
                     scorePair(first[i - 1], second[j - 1], scoring);
      int up = scoreMatrix[i - 1][j] + scoring.gapScore;
      int left = scoreMatrix[i][j - 1] + scoring.gapScore;
      scoreMatrix[i][j] = std::max({diagonal, up, left});
    }
  }
  return scoreMatrix;
}

char NeedlemanWunschAligner::buildMatchChar(char lhs, char rhs) const {
  return lhs == rhs ? '|' : ' ';
}

AlignmentResult NeedlemanWunschAligner::traceback(
    const std::string &first, const std::string &second,
    const ScoringScheme &scoring,
    const std::vector<std::vector<int>> &scoreMatrix) const {
  std::string alignedFirst;
  std::string alignedSecond;
  std::string matchLine;

  std::size_t i = first.size();
  std::size_t j = second.size();

  while (i > 0 || j > 0) {

    int currentScore = scoreMatrix[i][j];
    if (i > 0 && currentScore == scoreMatrix[i - 1][j] + scoring.gapScore) {
      alignedFirst.push_back(first[i - 1]);
      alignedSecond.push_back('-');
      matchLine.push_back(' ');
      --i;
    } else if (i > 0 && j > 0 &&
               currentScore ==
                   scoreMatrix[i - 1][j - 1] +
                       scorePair(first[i - 1], second[j - 1], scoring)) {
      alignedFirst.push_back(first[i - 1]);
      alignedSecond.push_back(second[j - 1]);
      matchLine.push_back(buildMatchChar(first[i - 1], second[j - 1]));
      --i;
      --j;
    } else if (j > 0 &&
               currentScore == scoreMatrix[i][j - 1] + scoring.gapScore) {
      alignedFirst.push_back('-');
      alignedSecond.push_back(second[j - 1]);
      matchLine.push_back(' ');
      --j;
    }
  }

  std::reverse(alignedFirst.begin(), alignedFirst.end());
  std::reverse(matchLine.begin(), matchLine.end());
  std::reverse(alignedSecond.begin(), alignedSecond.end());
  const int optimalScore = scoreMatrix[first.size()][second.size()];

  return AlignmentResult{optimalScore, alignedFirst, matchLine, alignedSecond};
}