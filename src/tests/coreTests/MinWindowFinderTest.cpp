#include <gtest/gtest.h>

#include "../../core/minWindow/MinWindowFinder.hpp"

class MinWindowFinderTest : public ::testing::Test {
protected:
  MinWindowFinder finder;
};

TEST_F(MinWindowFinderTest, ReturnsExampleResult) {
  EXPECT_EQ(finder.findMinimumWindow("GGCGACACCCACCATACAT", "TGT"),
            "GACACCCACCATACAT");
}

TEST_F(MinWindowFinderTest, ReturnsEmptyWhenPatternIsEmpty) {
  EXPECT_EQ(finder.findMinimumWindow("ACGT", ""), "");
}

TEST_F(MinWindowFinderTest, ReturnsEmptyWhenTextIsEmpty) {
  EXPECT_EQ(finder.findMinimumWindow("", "A"), "");
}

TEST_F(MinWindowFinderTest, ReturnsEmptyWhenPatternLongerThanText) {
  EXPECT_EQ(finder.findMinimumWindow("AC", "ACG"), "");
}

TEST_F(MinWindowFinderTest, ReturnsEmptyWhenNoWindowExists) {
  EXPECT_EQ(finder.findMinimumWindow("AAAA", "C"), "");
}

TEST_F(MinWindowFinderTest, ReturnsEmptyWhenNotEnoughDuplicates) {
  EXPECT_EQ(finder.findMinimumWindow("ACGT", "TT"), "");
}

TEST_F(MinWindowFinderTest, WholeTextIsMinimumWindow) {
  EXPECT_EQ(finder.findMinimumWindow("ACGT", "ACGT"), "ACGT");
}

TEST_F(MinWindowFinderTest, SingleCharacterPatternExists) {
  EXPECT_EQ(finder.findMinimumWindow("GGGAC", "A"), "A");
}

TEST_F(MinWindowFinderTest, SingleCharacterPatternDoesNotExist) {
  EXPECT_EQ(finder.findMinimumWindow("GGGAC", "T"), "");
}

TEST_F(MinWindowFinderTest, HandlesDuplicateCharactersInPattern) {
  EXPECT_EQ(finder.findMinimumWindow("AAACGT", "AAC"), "AAC");
}

TEST_F(MinWindowFinderTest, MinimumWindowAtBeginning) {
  EXPECT_EQ(finder.findMinimumWindow("ACGTGGGG", "ACG"), "ACG");
}

TEST_F(MinWindowFinderTest, MinimumWindowAtEnd) {
  EXPECT_EQ(finder.findMinimumWindow("GGGGACGT", "CGT"), "CGT");
}

TEST_F(MinWindowFinderTest, MinimumWindowInMiddle) {
  EXPECT_EQ(finder.findMinimumWindow("GGACGTTT", "ACG"), "GAC");
}

TEST_F(MinWindowFinderTest, ChoosesShortestWindowNotFirstValidWindow) {
  EXPECT_EQ(finder.findMinimumWindow("AAGCGTAC", "GTA"), "GTA");
}

TEST_F(MinWindowFinderTest, HandlesAllSameCharacters) {
  EXPECT_EQ(finder.findMinimumWindow("AAAAAA", "AAA"), "AAA");
}

TEST_F(MinWindowFinderTest, HandlesRepeatedRequiredCharactersSeparated) {
  EXPECT_EQ(finder.findMinimumWindow("TAAAGT", "TTG"), "TAAAGT");
}

TEST_F(MinWindowFinderTest, HandlesExtraCharactersBeforeAndAfter) {
  EXPECT_EQ(finder.findMinimumWindow("CCCCAAAGTCCCC", "AGT"), "AGT");
}

TEST_F(MinWindowFinderTest, HandlesPatternWithOneUniqueCharacter) {
  EXPECT_EQ(finder.findMinimumWindow("CCCTTTTAAA", "TTT"), "TTT");
}

TEST_F(MinWindowFinderTest, HandlesTextAndPatternSingleSameCharacter) {
  EXPECT_EQ(finder.findMinimumWindow("A", "A"), "A");
}

TEST_F(MinWindowFinderTest, HandlesTextAndPatternSingleDifferentCharacter) {
  EXPECT_EQ(finder.findMinimumWindow("A", "C"), "");
}

TEST_F(MinWindowFinderTest, PatternHasMissingCharacter) {
  EXPECT_EQ(finder.findMinimumWindow("ABC", "ABD"), "");
}

TEST_F(MinWindowFinderTest, PatternRequiresMoreCopiesThanTextHas) {
  EXPECT_EQ(finder.findMinimumWindow("ABBC", "AAB"), "");
}

TEST_F(MinWindowFinderTest, MinimumWindowDoesNotMatchPatternOrder) {
  EXPECT_EQ(finder.findMinimumWindow("BAC", "ABC"), "BAC");
}

TEST_F(MinWindowFinderTest, DuplicateCharactersInPatternWithGaps) {
  EXPECT_EQ(finder.findMinimumWindow("AACB", "AAB"), "AACB");
}

TEST_F(MinWindowFinderTest, SingleCharacterWithMultipleRequiredCopiesAndGaps) {
  EXPECT_EQ(finder.findMinimumWindow("ABABA", "AAA"), "ABABA");
}

TEST_F(MinWindowFinderTest, NoisyCharactersInsideAndOutsideWindow) {
  EXPECT_EQ(finder.findMinimumWindow("XABYCZ", "ABC"), "ABYC");
}

TEST_F(MinWindowFinderTest, AlphabetWithDigits) {
  EXPECT_EQ(finder.findMinimumWindow("123456", "345"), "345");
  EXPECT_EQ(finder.findMinimumWindow("1a2b3c", "abc"), "a2b3c");
}

TEST_F(MinWindowFinderTest, AlphabetWithSpecialCharacters) {
  EXPECT_EQ(finder.findMinimumWindow("!@#$%^&*()", "@#$"), "@#$");
  EXPECT_EQ(finder.findMinimumWindow("ab!cde!f", "!!"), "!cde!");
}

TEST_F(MinWindowFinderTest, AlphabetWithUnicode) {
  EXPECT_EQ(finder.findMinimumWindow("абвгдеёжз", "вгд"), "вгд");
  EXPECT_EQ(finder.findMinimumWindow("aбвгd", "бв"), "бв");
  EXPECT_EQ(finder.findMinimumWindow("§©™", "©"), "©");
}