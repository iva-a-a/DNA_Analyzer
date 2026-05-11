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

TEST_F(RabinKarpSearcherTest, ReturnsEmptyWhenHashCollisionIsNotRealMatch) {
  std::string text;
  text.push_back(static_cast<char>(235));
  text.push_back(static_cast<char>(167));
  text.push_back(static_cast<char>(0));
  text.push_back(static_cast<char>(114));

  std::string pattern;
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(232));
  pattern.push_back(static_cast<char>(0));

  EXPECT_TRUE(searcher.find(text, pattern).empty());
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithDigitsAlphabet) {
  std::vector<std::size_t> expected = {2, 8};

  EXPECT_EQ(searcher.find("991234991234", "1234"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithLowercaseAlphabet) {
  std::vector<std::size_t> expected = {0, 6};

  EXPECT_EQ(searcher.find("abcxyzabc", "abc"), expected);
}

TEST_F(RabinKarpSearcherTest,
       FindsPatternWithMixedCaseAlphabetIsCaseSensitive) {
  std::vector<std::size_t> expected = {3};

  EXPECT_EQ(searcher.find("AbCaBcABC", "aBc"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithWhitespaceCharacters) {
  std::vector<std::size_t> expected = {1, 5};

  EXPECT_EQ(searcher.find("A B C B C", " B"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithTabsAndNewlines) {
  std::vector<std::size_t> expected = {1, 5};

  EXPECT_EQ(searcher.find("A\tB\nA\tB\n", "\tB\n"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithPunctuationAlphabet) {
  std::vector<std::size_t> expected = {0, 6};

  EXPECT_EQ(searcher.find("!?.,;:!?.,;:", "!?.,;:"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithMixedAsciiAlphabet) {
  std::vector<std::size_t> expected = {2, 9};

  EXPECT_EQ(searcher.find("xxA1!bC??A1!bC", "A1!bC"), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternContainingNullByte) {
  std::string text = std::string("AA", 2) + std::string("\0BC", 3) +
                     std::string("ZZ", 2) + std::string("\0BC", 3);
  std::string pattern = std::string("\0BC", 3);

  std::vector<std::size_t> expected = {2, 7};

  EXPECT_EQ(searcher.find(text, pattern), expected);
}

TEST_F(RabinKarpSearcherTest, FindsPatternWithHighBitBytes) {
  std::string text;
  text.push_back('A');
  text.push_back(static_cast<char>(200));
  text.push_back(static_cast<char>(201));
  text.push_back('B');
  text.push_back(static_cast<char>(200));
  text.push_back(static_cast<char>(201));

  std::string pattern;
  pattern.push_back(static_cast<char>(200));
  pattern.push_back(static_cast<char>(201));

  std::vector<std::size_t> expected = {1, 4};

  EXPECT_EQ(searcher.find(text, pattern), expected);
}

TEST_F(RabinKarpSearcherTest, LongTextFindsMatchNearBeginningMiddleAndEnd) {
  std::string pattern = "ACGTXYZ";
  std::string text = std::string(1000, 'A') + pattern + std::string(2000, 'B') +
                     pattern + std::string(3000, 'C') + pattern;

  std::vector<std::size_t> expected = {1000, 1000 + pattern.size() + 2000,
                                       1000 + pattern.size() + 2000 +
                                           pattern.size() + 3000};

  EXPECT_EQ(searcher.find(text, pattern), expected);
}

TEST_F(RabinKarpSearcherTest, LongTextWithManyOverlappingMatches) {
  std::string text(1000, 'A');
  std::string pattern(50, 'A');

  std::vector<std::size_t> expected;
  for (std::size_t i = 0; i <= text.size() - pattern.size(); ++i) {
    expected.push_back(i);
  }

  EXPECT_EQ(searcher.find(text, pattern), expected);
}

TEST_F(RabinKarpSearcherTest, LongTextWithNoMatches) {
  std::string text =
      std::string(5000, 'A') + std::string(5000, 'C') + std::string(5000, 'G');

  EXPECT_TRUE(searcher.find(text, "TTTTTT").empty());
}

TEST_F(RabinKarpSearcherTest, LongPatternEqualsLongText) {
  std::string text = std::string(1000, 'A') + std::string(1000, 'C') +
                     std::string(1000, 'G') + std::string(1000, 'T');

  std::vector<std::size_t> expected = {0};

  EXPECT_EQ(searcher.find(text, text), expected);
}

TEST_F(RabinKarpSearcherTest, LongPatternAlmostEqualsTextButDiffersAtEnd) {
  std::string text = std::string(1000, 'A') + std::string(1000, 'C') +
                     std::string(1000, 'G') + std::string(1000, 'T');

  std::string pattern = text;
  pattern.back() = 'A';

  EXPECT_TRUE(searcher.find(text, pattern).empty());
}

TEST_F(RabinKarpSearcherTest, LongTextChoosesAllRealMatchesAmongNoise) {
  std::string pattern = "ABRACADABRA";
  std::string text = std::string(777, 'X') + pattern + std::string(333, 'Y') +
                     "ABRACADABRB" + std::string(444, 'Z') + pattern;

  std::vector<std::size_t> expected = {
      777,
      777 + pattern.size() + 333 + std::string("ABRACADABRB").size() + 444};

  EXPECT_EQ(searcher.find(text, pattern), expected);
}

TEST_F(RabinKarpSearcherTest, CollisionLikeWindowDoesNotProduceFalsePositive) {
  std::string text;
  text.push_back(static_cast<char>(235));
  text.push_back(static_cast<char>(167));
  text.push_back(static_cast<char>(0));
  text.push_back(static_cast<char>(114));
  text += "REAL";

  std::string pattern = "REAL";

  std::vector<std::size_t> expected = {4};

  EXPECT_EQ(searcher.find(text, pattern), expected);
}

TEST_F(RabinKarpSearcherTest, MultipleCollisionLikeWindowsDoNotProduceMatches) {
  std::string collisionLikeWindow;
  collisionLikeWindow.push_back(static_cast<char>(235));
  collisionLikeWindow.push_back(static_cast<char>(167));
  collisionLikeWindow.push_back(static_cast<char>(0));
  collisionLikeWindow.push_back(static_cast<char>(114));

  std::string pattern;
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(232));
  pattern.push_back(static_cast<char>(0));

  std::string text = collisionLikeWindow + "A" + collisionLikeWindow + "B" +
                     collisionLikeWindow;

  EXPECT_TRUE(searcher.find(text, pattern).empty());
}

TEST_F(RabinKarpSearcherTest, CollisionLikeWindowsAroundRealMatch) {
  std::string collisionLikeWindow;
  collisionLikeWindow.push_back(static_cast<char>(235));
  collisionLikeWindow.push_back(static_cast<char>(167));
  collisionLikeWindow.push_back(static_cast<char>(0));
  collisionLikeWindow.push_back(static_cast<char>(114));

  std::string pattern;
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(232));
  pattern.push_back(static_cast<char>(0));

  std::string text =
      collisionLikeWindow + "X" + pattern + "Y" + collisionLikeWindow;

  std::vector<std::size_t> expected = {5};

  EXPECT_EQ(searcher.find(text, pattern), expected);
}

TEST_F(RabinKarpSearcherTest, LongTextWithCollisionLikeWindowsAndRealMatches) {
  std::string collisionLikeWindow;
  collisionLikeWindow.push_back(static_cast<char>(235));
  collisionLikeWindow.push_back(static_cast<char>(167));
  collisionLikeWindow.push_back(static_cast<char>(0));
  collisionLikeWindow.push_back(static_cast<char>(114));

  std::string pattern;
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(0));
  pattern.push_back(static_cast<char>(232));
  pattern.push_back(static_cast<char>(0));

  std::string text = std::string(1000, 'A') + collisionLikeWindow +
                     std::string(500, 'B') + pattern + std::string(500, 'C') +
                     collisionLikeWindow + std::string(500, 'D') + pattern;

  std::vector<std::size_t> expected = {1000 + collisionLikeWindow.size() + 500,
                                       1000 + collisionLikeWindow.size() + 500 +
                                           pattern.size() + 500 +
                                           collisionLikeWindow.size() + 500};

  EXPECT_EQ(searcher.find(text, pattern), expected);
}