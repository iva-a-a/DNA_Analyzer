#include <gtest/gtest.h>

#include "../../core/rabinKarp/RabinKarpSearcher.hpp"

class RabinKarpSearcherTest : public ::testing::Test {
protected:
  RabinKarpSearcher searcher;
};

TEST_F(RabinKarpSearcherTest, ReturnsEmptyWhenPatternIsEmpty) {
  EXPECT_TRUE(searcher.find("ACGT", "").empty());
}

TEST_F(RabinKarpSearcherTest, ReturnsEmptyWhenTextIsEmpty) {
  EXPECT_TRUE(searcher.find("", "A").empty());
}

TEST_F(RabinKarpSearcherTest, ReturnsEmptyWhenBothTextAndPatternAreEmpty) {
  EXPECT_TRUE(searcher.find("", "").empty());
}

TEST_F(RabinKarpSearcherTest, ReturnsEmptyWhenPatternLongerThanText) {
  EXPECT_TRUE(searcher.find("ACG", "ACGT").empty());
}

TEST_F(RabinKarpSearcherTest, FindsSingleMatchAtBeginning) {
  std::vector<std::size_t> expected = {0};

  EXPECT_EQ(searcher.find("ACGT", "AC"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsSingleMatchInMiddle) {
  std::vector<std::size_t> expected = {2};

  EXPECT_EQ(searcher.find("TTACGG", "AC"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsSingleMatchAtEnd) {
  std::vector<std::size_t> expected = {4};

  EXPECT_EQ(searcher.find("TTTTAC", "AC"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsFullTextMatch) {
  std::vector<std::size_t> expected = {0};

  EXPECT_EQ(searcher.find("ACGT", "ACGT"), expected);
}

TEST_F(RabinKarpSearcherTest, ReturnsEmptyWhenNoMatch) {
  EXPECT_TRUE(searcher.find("ACGTACGT", "TTT").empty());
}

TEST_F(RabinKarpSearcherTest, FindsMultipleNonOverlappingMatches) {
  std::vector<std::size_t> expected = {0, 4, 8};

  EXPECT_EQ(searcher.find("ACGTACGTACGT", "ACGT"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsMultipleOverlappingMatches) {
  std::vector<std::size_t> expected = {0, 1, 2};

  EXPECT_EQ(searcher.find("AAAA", "AA"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsSingleCharacterPattern) {
  std::vector<std::size_t> expected = {0, 4};

  EXPECT_EQ(searcher.find("ACGTA", "A"), expected);
}

TEST_F(RabinKarpSearcherTest,
       FindsAllCharactersWhenSingleCharacterTextAndPatternMatch) {
  std::vector<std::size_t> expected = {0};

  EXPECT_EQ(searcher.find("A", "A"), expected);
}

TEST_F(RabinKarpSearcherTest,
       ReturnsEmptyWhenSingleCharacterTextAndPatternDoNotMatch) {
  EXPECT_TRUE(searcher.find("A", "C").empty());
}

TEST_F(RabinKarpSearcherTest,
       HandlesPatternLengthOneWithoutRollingHashRecalculation) {
  std::vector<std::size_t> expected = {1, 3, 5};

  EXPECT_EQ(searcher.find("CACACA", "A"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsRepeatedPatternWithShiftedWindows) {
  std::vector<std::size_t> expected = {1, 6};

  EXPECT_EQ(searcher.find("TACGTTACGT", "ACGT"), expected);
}

TEST_F(RabinKarpSearcherTest, DoesNotMatchPartialPrefixOnly) {
  EXPECT_TRUE(searcher.find("ACGA", "ACGT").empty());
}

TEST_F(RabinKarpSearcherTest,
       FindsPatternWhenTextContainsInvalidDnaCharacters) {
  std::vector<std::size_t> expected = {2};

  EXPECT_EQ(searcher.find("XXACGTXX", "ACGT"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithSpecialCharacters) {
  std::vector<std::size_t> expected = {1, 4};

  EXPECT_EQ(searcher.find("A$CA$C", "$C"), expected);
}

TEST_F(RabinKarpSearcherTest, HandlesLongText) {
  std::string text(100, 'A');
  text += "CGT";
  text += std::string(100, 'A');

  std::vector<std::size_t> expected = {100};

  EXPECT_EQ(searcher.find(text, "CGT"), expected);
}

// TODO добавить тесты с разными алфавитами, сложные случаи с коллизиями, очень
// длинные строки