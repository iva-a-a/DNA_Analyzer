#include <gtest/gtest.h>

#include "../../core/kSimilarity/KSimilaritySolver.hpp"

class KSimilaritySolverTest : public ::testing::Test {
protected:
  KSimilaritySolver solver;
};

TEST_F(KSimilaritySolverTest, EmptyStrings) {
  EXPECT_EQ(solver.findMinimumSwaps("", ""), 0);
}

TEST_F(KSimilaritySolverTest, EqualSingleCharacterStrings) {
  EXPECT_EQ(solver.findMinimumSwaps("A", "A"), 0);
}

TEST_F(KSimilaritySolverTest, DifferentSingleCharacterStringsAreNotAnagrams) {
  EXPECT_EQ(solver.findMinimumSwaps("A", "B"), -1);
}

TEST_F(KSimilaritySolverTest, EqualStringsRequireZeroSwaps) {
  EXPECT_EQ(solver.findMinimumSwaps("ACGT", "ACGT"), 0);
}

TEST_F(KSimilaritySolverTest, DifferentLengthsAreNotAnagrams) {
  EXPECT_EQ(solver.findMinimumSwaps("ABC", "ABCD"), -1);
}

TEST_F(KSimilaritySolverTest, SameLengthButNotAnagrams) {
  EXPECT_EQ(solver.findMinimumSwaps("AABC", "ABCD"), -1);
}

TEST_F(KSimilaritySolverTest, OneDirectSwap) {
  EXPECT_EQ(solver.findMinimumSwaps("AB", "BA"), 1);
  EXPECT_EQ(solver.findMinimumSwaps("AB", "BA"), 1);
}

TEST_F(KSimilaritySolverTest, OneSwapInsideLongerString) {
  EXPECT_EQ(solver.findMinimumSwaps("ABCDE", "ADCBE"), 1);
}

TEST_F(KSimilaritySolverTest, ThreeCycleRequiresTwoSwaps) {
  EXPECT_EQ(solver.findMinimumSwaps("ABC", "BCA"), 2);
}

TEST_F(KSimilaritySolverTest, FourCycleRequiresThreeSwaps) {
  EXPECT_EQ(solver.findMinimumSwaps("ABCD", "BCDA"), 3);
}

TEST_F(KSimilaritySolverTest, MultipleIndependentSwaps) {
  EXPECT_EQ(solver.findMinimumSwaps("AABB", "BBAA"), 2);
}

TEST_F(KSimilaritySolverTest, RepeatedCharactersWithOneSwap) {
  EXPECT_EQ(solver.findMinimumSwaps("AAB", "ABA"), 1);
}

TEST_F(KSimilaritySolverTest, RepeatedCharactersWithTwoSwaps) {
  EXPECT_EQ(solver.findMinimumSwaps("AABC", "BCAA"), 2);
}

TEST_F(KSimilaritySolverTest, RepeatedCharactersNonTrivialCase) {
  EXPECT_EQ(solver.findMinimumSwaps("ABCA", "CABA"), 2);
}

TEST_F(KSimilaritySolverTest, SampleFromStatement) {
  EXPECT_EQ(solver.findMinimumSwaps("GGCGACACC", "AGCCGCGAC"), 3);
}

TEST_F(KSimilaritySolverTest, WorksWithArbitraryAlphabetDigits) {
  EXPECT_EQ(solver.findMinimumSwaps("112233", "332211"), 2);
}

TEST_F(KSimilaritySolverTest, WorksWithArbitraryAlphabetLetters) {
  EXPECT_EQ(solver.findMinimumSwaps("aabbcc", "ccbbaa"), 2);
}

TEST_F(KSimilaritySolverTest, CaseSensitiveCharacters) {
  EXPECT_EQ(solver.findMinimumSwaps("Aa", "aA"), 1);
}

TEST_F(KSimilaritySolverTest, ManySameCharactersAlreadyEqual) {
  EXPECT_EQ(solver.findMinimumSwaps("AAAAAA", "AAAAAA"), 0);
}

TEST_F(KSimilaritySolverTest, ManySameCharactersNotAnagram) {
  EXPECT_EQ(solver.findMinimumSwaps("AAAAAA", "AAAAAB"), -1);
}

TEST_F(KSimilaritySolverTest, ReverseUniqueString) {
  EXPECT_EQ(solver.findMinimumSwaps("ABCDE", "EDCBA"), 2);
}

TEST_F(KSimilaritySolverTest, ReverseEvenUniqueString) {
  EXPECT_EQ(solver.findMinimumSwaps("ABCDEF", "FEDCBA"), 3);
}

TEST_F(KSimilaritySolverTest, ComplexRepeatedCharacters) {
  EXPECT_EQ(solver.findMinimumSwaps("AAABBBCCC", "CCCAAABBB"), 6);
}

TEST_F(KSimilaritySolverTest, TwoCharactersAlreadyEqual) {
  EXPECT_EQ(solver.findMinimumSwaps("AA", "AA"), 0);
}

TEST_F(KSimilaritySolverTest, TwoCharactersNotAnagram) {
  EXPECT_EQ(solver.findMinimumSwaps("AA", "AB"), -1);
}

TEST_F(KSimilaritySolverTest, NeedsChoiceBetweenSeveralSameLetters) {
  EXPECT_EQ(solver.findMinimumSwaps("AABC", "ABCA"), 2);
}

TEST_F(KSimilaritySolverTest, RepeatedLettersWithSeveralPossibleSwaps) {
  EXPECT_EQ(solver.findMinimumSwaps("ABBA", "BAAB"), 2);
}

TEST_F(KSimilaritySolverTest, LongerRepeatedLettersCase) {
  EXPECT_EQ(solver.findMinimumSwaps("AABBCC", "CCAABB"), 4);
}

TEST_F(KSimilaritySolverTest, AlreadySortedToReverseWithDuplicates) {
  EXPECT_EQ(solver.findMinimumSwaps("AABBCCDD", "DDCCBBAA"), 4);
}

TEST_F(KSimilaritySolverTest, DigitsWithCycle) {
  EXPECT_EQ(solver.findMinimumSwaps("1234", "2341"), 3);
}

TEST_F(KSimilaritySolverTest, SymbolsAlphabet) {
  EXPECT_EQ(solver.findMinimumSwaps("!@#", "#!@"), 2);
}

TEST_F(KSimilaritySolverTest, WhitespaceCharacters) {
  EXPECT_EQ(solver.findMinimumSwaps("A B", "BA "), 2);
}

TEST_F(KSimilaritySolverTest, MixedAlphabet) {
  EXPECT_EQ(solver.findMinimumSwaps("A1!a", "!aA1"), 2);
}

TEST_F(KSimilaritySolverTest, EmptyFirstButNonEmptySecond) {
  EXPECT_EQ(solver.findMinimumSwaps("", "A"), -1);
}

TEST_F(KSimilaritySolverTest, NonEmptyFirstButEmptySecond) {
  EXPECT_EQ(solver.findMinimumSwaps("A", ""), -1);
}

TEST_F(KSimilaritySolverTest, OneCharacterEqual) {
  EXPECT_EQ(solver.findMinimumSwaps("A", "A"), 0);
}

TEST_F(KSimilaritySolverTest, OneCharacterDifferent) {
  EXPECT_EQ(solver.findMinimumSwaps("A", "B"), -1);
  EXPECT_EQ(solver.findMinimumSwaps("A", "B"), -1);
}

TEST_F(KSimilaritySolverTest, SameLengthDifferentCounts) {
  EXPECT_EQ(solver.findMinimumSwaps("AAB", "ABB"), -1);
}

TEST_F(KSimilaritySolverTest, SameLengthCompletelyDifferentCounts) {
  EXPECT_EQ(solver.findMinimumSwaps("ABC", "DEF"), -1);
}

TEST_F(KSimilaritySolverTest, AlreadyEqualLongString) {
  EXPECT_EQ(solver.findMinimumSwaps("ABCDEF", "ABCDEF"), 0);
}

TEST_F(KSimilaritySolverTest, WhileStopsImmediatelyInSolve) {
  EXPECT_EQ(solver.findMinimumSwaps("BA", "AB"), 1);
  EXPECT_EQ(solver.findMinimumSwaps("BA", "AB"), 1);
}

TEST_F(KSimilaritySolverTest, WhileSkipsSeveralCorrectCharactersInSolve) {
  EXPECT_EQ(solver.findMinimumSwaps("AACB", "AABC"), 1);
}

TEST_F(KSimilaritySolverTest, MultipleCandidateSwaps) {
  EXPECT_EQ(solver.findMinimumSwaps("AABC", "BCAA"), 2);
}

TEST_F(KSimilaritySolverTest, ReverseFourUnique) {
  EXPECT_EQ(solver.findMinimumSwaps("ABCD", "DCBA"), 2);
}

TEST_F(KSimilaritySolverTest, TwoBlocksSwap) {
  EXPECT_EQ(solver.findMinimumSwaps("AAAABBBB", "BBBBAAAA"), 4);
}

TEST_F(KSimilaritySolverTest, DoesNotModifyInputs) {
  std::string first = "ABC";
  std::string second = "BCA";
  std::string first_copy = first;
  std::string second_copy = second;
  solver.findMinimumSwaps(first, second);
  EXPECT_EQ(first, first_copy);
  EXPECT_EQ(second, second_copy);
}