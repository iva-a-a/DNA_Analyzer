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

// TODO добавить тесты с другими параметрами, другой алфавит, сложные длинные
// строки, несколько оптимальных выравниваний