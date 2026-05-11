#include <gtest/gtest.h>

#include "../../core/needlemanWunsch/NeedlemanWunschAligner.hpp"

class NeedlemanWunschAlignerTest : public ::testing::Test {
protected:
  NeedlemanWunschAligner aligner;
  ScoringScheme scoring{1, -1, -2};
};

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenBothSequencesEmpty_ReturnsZero) {
  EXPECT_EQ(aligner.computeScore("", "", scoring), 0);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenFirstSequenceEmpty_ReturnsOnlyGaps) {
  EXPECT_EQ(aligner.computeScore("", "ACGT", scoring), -8);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenSecondSequenceEmpty_ReturnsOnlyGaps) {
  EXPECT_EQ(aligner.computeScore("ACGT", "", scoring), -8);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenSingleCharactersMatch_ReturnsMatchScore) {
  EXPECT_EQ(aligner.computeScore("A", "A", scoring), 1);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenSingleCharactersMismatch_ReturnsMismatchScore) {
  EXPECT_EQ(aligner.computeScore("A", "G", scoring), -1);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenSequencesFullyMatch_ReturnsSumOfMatches) {
  EXPECT_EQ(aligner.computeScore("ACGT", "ACGT", scoring), 4);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenSequencesFullyMismatch_ReturnsSumOfMismatches) {
  EXPECT_EQ(aligner.computeScore("AAAA", "CCCC", scoring), -4);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenGapIsOptimal_ReturnsCorrectScore) {
  EXPECT_EQ(aligner.computeScore("AC", "A", scoring), -1);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenExampleFromTask_ReturnsTen) {
  EXPECT_EQ(aligner.computeScore("GGGCGACACTCCACCATAGA", "GGCGACACCCACCATACAT",
                                 scoring),
            10);
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenBothSequencesEmpty_ReturnsEmptyAlignment) {
  AlignmentResult result = aligner.align("", "", scoring);

  EXPECT_EQ(result.score, 0);
  EXPECT_EQ(result.alignedFirst, "");
  EXPECT_EQ(result.matchLine, "");
  EXPECT_EQ(result.alignedSecond, "");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenFirstSequenceEmpty_AddsGapsToFirst) {
  AlignmentResult result = aligner.align("", "ACGT", scoring);

  EXPECT_EQ(result.score, -8);
  EXPECT_EQ(result.alignedFirst, "----");
  EXPECT_EQ(result.matchLine, "    ");
  EXPECT_EQ(result.alignedSecond, "ACGT");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenSecondSequenceEmpty_AddsGapsToSecond) {
  AlignmentResult result = aligner.align("ACGT", "", scoring);

  EXPECT_EQ(result.score, -8);
  EXPECT_EQ(result.alignedFirst, "ACGT");
  EXPECT_EQ(result.matchLine, "    ");
  EXPECT_EQ(result.alignedSecond, "----");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenSingleCharactersMatch_ReturnsMatchLine) {
  AlignmentResult result = aligner.align("A", "A", scoring);

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst, "A");
  EXPECT_EQ(result.matchLine, "|");
  EXPECT_EQ(result.alignedSecond, "A");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenSingleCharactersMismatch_ReturnsSpaceInMatchLine) {
  AlignmentResult result = aligner.align("A", "G", scoring);

  EXPECT_EQ(result.score, -1);
  EXPECT_EQ(result.alignedFirst, "A");
  EXPECT_EQ(result.matchLine, " ");
  EXPECT_EQ(result.alignedSecond, "G");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenSequencesFullyMatch_ReturnsSameSequences) {
  AlignmentResult result = aligner.align("ACGT", "ACGT", scoring);

  EXPECT_EQ(result.score, 4);
  EXPECT_EQ(result.alignedFirst, "ACGT");
  EXPECT_EQ(result.matchLine, "||||");
  EXPECT_EQ(result.alignedSecond, "ACGT");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenGapInSecondSequenceIsOptimal_ReturnsGapInSecond) {
  AlignmentResult result = aligner.align("AC", "A", scoring);

  EXPECT_EQ(result.score, -1);
  EXPECT_EQ(result.alignedFirst, "AC");
  EXPECT_EQ(result.matchLine, "| ");
  EXPECT_EQ(result.alignedSecond, "A-");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenGapInFirstSequenceIsOptimal_ReturnsGapInFirst) {
  AlignmentResult result = aligner.align("A", "AC", scoring);

  EXPECT_EQ(result.score, -1);
  EXPECT_EQ(result.alignedFirst, "A-");
  EXPECT_EQ(result.matchLine, "| ");
  EXPECT_EQ(result.alignedSecond, "AC");
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenExampleFromTask_ReturnsExpectedAlignment) {
  AlignmentResult result =
      aligner.align("GGGCGACACTCCACCATAGA", "GGCGACACCCACCATACAT", scoring);

  EXPECT_EQ(result.score, 10);
  EXPECT_EQ(result.alignedFirst, "GGGCGACACTCCACCATAGA-");
  EXPECT_EQ(result.matchLine, "|| |||||| |||||||| | ");
  EXPECT_EQ(result.alignedSecond, "GG-CGACAC-CCACCATACAT");
}

TEST_F(NeedlemanWunschAlignerTest, Align_ResultStringsAlwaysHaveSameLength) {
  AlignmentResult result = aligner.align("ACGT", "AGT", scoring);

  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WithDifferentScoringScheme_PrefersMismatchOverTwoGaps) {
  ScoringScheme customScoring{2, -1, -3};

  EXPECT_EQ(aligner.computeScore("A", "G", customScoring), -1);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WithDifferentScoringScheme_PenalizesGapsMoreStrongly) {
  ScoringScheme customScoring{3, -2, -5};

  EXPECT_EQ(aligner.computeScore("ACGT", "ACG", customScoring), 4);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WithDifferentScoringScheme_AllMatches) {
  ScoringScheme customScoring{5, -4, -6};

  EXPECT_EQ(aligner.computeScore("ACGTACGT", "ACGTACGT", customScoring), 40);
}

TEST_F(NeedlemanWunschAlignerTest, ComputeScore_WithZeroMismatchPenalty) {
  ScoringScheme customScoring{2, 0, -3};

  EXPECT_EQ(aligner.computeScore("AAAA", "TTTT", customScoring), 0);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WithPositiveMismatchScoreStillPrefersMatches) {
  ScoringScheme customScoring{3, 1, -2};

  EXPECT_EQ(aligner.computeScore("ABCD", "ABXY", customScoring), 8);
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WithDifferentScoringScheme_ReturnsExpectedScoreAndAlignment) {
  ScoringScheme customScoring{2, -1, -3};

  AlignmentResult result = aligner.align("AC", "AGC", customScoring);

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WithHighGapPenaltyAvoidsUnnecessaryGaps) {
  ScoringScheme customScoring{2, -1, -10};

  AlignmentResult result = aligner.align("AAAA", "TTTT", customScoring);

  EXPECT_EQ(result.score, -4);
  EXPECT_EQ(result.alignedFirst, "AAAA");
  EXPECT_EQ(result.matchLine, "    ");
  EXPECT_EQ(result.alignedSecond, "TTTT");
}

TEST_F(NeedlemanWunschAlignerTest, ComputeScore_WithDigitsAlphabet) {
  EXPECT_EQ(aligner.computeScore("12345", "12355", scoring), 3);
}

TEST_F(NeedlemanWunschAlignerTest, Align_WithDigitsAlphabet) {
  AlignmentResult result = aligner.align("1234", "124", scoring);

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest, ComputeScore_WithSpecialCharactersAlphabet) {
  EXPECT_EQ(aligner.computeScore("!@#$", "!@%$", scoring), 2);
}

TEST_F(NeedlemanWunschAlignerTest, Align_WithSpecialCharactersAlphabet) {
  AlignmentResult result = aligner.align("!@#$", "!@$", scoring);

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest, ComputeScore_WithMixedAlphabet) {
  EXPECT_EQ(aligner.computeScore("A1!b", "A2!b", scoring), 2);
}

TEST_F(NeedlemanWunschAlignerTest, Align_WithMixedAlphabet) {
  AlignmentResult result = aligner.align("A1!b", "A!b", scoring);

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WithLowercaseAlphabet_IsCaseSensitive) {
  EXPECT_EQ(aligner.computeScore("abc", "ABC", scoring), -3);
}

TEST_F(NeedlemanWunschAlignerTest, ComputeScore_WithLongIdenticalSequences) {
  std::string first = std::string(100, 'A') + std::string(100, 'C') +
                      std::string(100, 'G') + std::string(100, 'T');

  EXPECT_EQ(aligner.computeScore(first, first, scoring), 400);
}

TEST_F(NeedlemanWunschAlignerTest, Align_WithLongIdenticalSequences) {
  std::string sequence = std::string(50, 'A') + std::string(50, 'C') +
                         std::string(50, 'G') + std::string(50, 'T');

  AlignmentResult result = aligner.align(sequence, sequence, scoring);

  EXPECT_EQ(result.score, 200);
  EXPECT_EQ(result.alignedFirst, sequence);
  EXPECT_EQ(result.matchLine, std::string(200, '|'));
  EXPECT_EQ(result.alignedSecond, sequence);
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WithLongSequenceAndOneLargeDeletion) {
  std::string first =
      std::string(100, 'A') + std::string(50, 'C') + std::string(100, 'G');

  std::string second = std::string(100, 'A') + std::string(100, 'G');

  EXPECT_EQ(aligner.computeScore(first, second, scoring), 100);
}

TEST_F(NeedlemanWunschAlignerTest, Align_WithLongSequenceAndOneLargeDeletion) {
  std::string first =
      std::string(30, 'A') + std::string(20, 'C') + std::string(30, 'G');

  std::string second = std::string(30, 'A') + std::string(30, 'G');

  AlignmentResult result = aligner.align(first, second, scoring);

  EXPECT_EQ(result.score, 20);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest, ComputeScore_WithLongComplexRepeatedBlocks) {
  std::string first = std::string(40, 'A') + std::string(40, 'C') +
                      std::string(40, 'G') + std::string(40, 'T');

  std::string second = std::string(40, 'A') + std::string(20, 'C') +
                       std::string(40, 'G') + std::string(20, 'T');

  EXPECT_EQ(aligner.computeScore(first, second, scoring), 40);
}

TEST_F(NeedlemanWunschAlignerTest, Align_WithLongComplexRepeatedBlocks) {
  std::string first = std::string(40, 'A') + std::string(40, 'C') +
                      std::string(40, 'G') + std::string(40, 'T');

  std::string second = std::string(40, 'A') + std::string(20, 'C') +
                       std::string(40, 'G') + std::string(20, 'T');

  AlignmentResult result = aligner.align(first, second, scoring);

  EXPECT_EQ(result.score, 40);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest, ComputeScore_WithAlternatingLongSequences) {
  std::string first;
  std::string second;

  for (int i = 0; i < 100; ++i) {
    first += "AC";
    second += "CA";
  }

  EXPECT_EQ(aligner.computeScore(first, second, scoring), 195);
}

TEST_F(NeedlemanWunschAlignerTest, Align_WithAlternatingLongSequences) {
  std::string first;
  std::string second;

  for (int i = 0; i < 50; ++i) {
    first += "AC";
    second += "CA";
  }

  AlignmentResult result = aligner.align(first, second, scoring);

  EXPECT_EQ(result.score, 95);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenSeveralOptimalAlignmentsExist_SameScore) {
  ScoringScheme tieScoring{1, -1, -1};

  EXPECT_EQ(aligner.computeScore("A", "AA", tieScoring), 0);
}

TEST_F(
    NeedlemanWunschAlignerTest,
    Align_WhenSeveralOptimalAlignmentsExist_SingleGapMayBePlacedDifferently) {
  ScoringScheme tieScoring{1, -1, -1};

  AlignmentResult result = aligner.align("A", "AA", tieScoring);

  EXPECT_EQ(result.score, 0);
  EXPECT_EQ(result.alignedFirst.size(), size_t{2});
  EXPECT_EQ(result.alignedSecond.size(), size_t{2});
  EXPECT_EQ(result.matchLine.size(), size_t{2});

  EXPECT_TRUE(result.alignedFirst == "A-" || result.alignedFirst == "-A");
  EXPECT_EQ(result.alignedSecond, "AA");
}

TEST_F(NeedlemanWunschAlignerTest,
       ComputeScore_WhenSeveralOptimalAlignmentsExist_RepeatedLetters) {
  ScoringScheme tieScoring{1, -1, -1};

  EXPECT_EQ(aligner.computeScore("AA", "AAA", tieScoring), 1);
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenSeveralOptimalAlignmentsExist_RepeatedLetters) {
  ScoringScheme tieScoring{1, -1, -1};

  AlignmentResult result = aligner.align("AA", "AAA", tieScoring);

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst.size(), size_t{3});
  EXPECT_EQ(result.alignedSecond.size(), size_t{3});
  EXPECT_EQ(result.matchLine.size(), size_t{3});
  EXPECT_EQ(result.alignedSecond, "AAA");

  int gapsInFirst = 0;
  int matches = 0;

  for (size_t i = 0; i < result.alignedFirst.size(); ++i) {
    if (result.alignedFirst[i] == '-') {
      ++gapsInFirst;
    }
    if (result.matchLine[i] == '|') {
      ++matches;
    }
  }

  EXPECT_EQ(gapsInFirst, 1);
  EXPECT_EQ(matches, 2);
}

TEST_F(NeedlemanWunschAlignerTest,
       Align_WhenSeveralOptimalAlignmentsExist_ChecksScoreNotExactTraceback) {
  ScoringScheme tieScoring{1, -1, -1};

  AlignmentResult result = aligner.align("AB", "AAB", tieScoring);

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst.size(), result.alignedSecond.size());
  EXPECT_EQ(result.alignedFirst.size(), result.matchLine.size());

  int gapsInFirst = 0;
  int matches = 0;

  for (size_t i = 0; i < result.alignedFirst.size(); ++i) {
    if (result.alignedFirst[i] == '-') {
      ++gapsInFirst;
    }
    if (result.matchLine[i] == '|') {
      ++matches;
    }
  }

  EXPECT_EQ(gapsInFirst, 1);
  EXPECT_EQ(matches, 2);
}