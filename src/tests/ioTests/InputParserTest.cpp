#include <gtest/gtest.h>

#include "../../core/common/errors/Errors.hpp"
#include "../../io/parser/InputParser.hpp"

using namespace InputParser;

TEST(InputParserParseAlignmentInputTest, ValidInput) {
  const std::vector<std::string> lines = {
      "1 -1 -2",
      "ACGT",
      "AGT",
  };

  const AlignmentInput input = parseAlignmentInput(lines);

  EXPECT_EQ(input.scoring.matchScore, 1);
  EXPECT_EQ(input.scoring.mismatchScore, -1);
  EXPECT_EQ(input.scoring.gapScore, -2);
  EXPECT_EQ(input.first, "ACGT");
  EXPECT_EQ(input.second, "AGT");
}

TEST(InputParserParseAlignmentInputTest, ValidInputWithExtraSpaces) {
  const std::vector<std::string> lines = {
      "   10    -20    30   ",
      "A",
      "B",
  };

  const AlignmentInput input = parseAlignmentInput(lines);

  EXPECT_EQ(input.scoring.matchScore, 10);
  EXPECT_EQ(input.scoring.mismatchScore, -20);
  EXPECT_EQ(input.scoring.gapScore, 30);
  EXPECT_EQ(input.first, "A");
  EXPECT_EQ(input.second, "B");
}

TEST(InputParserParseAlignmentInputTest, AllowsExtraTokensAfterThreeIntegers) {
  const std::vector<std::string> lines = {
      "1 2 3 extra",
      "ABC",
      "DEF",
  };

  const AlignmentInput input = parseAlignmentInput(lines);

  EXPECT_EQ(input.scoring.matchScore, 1);
  EXPECT_EQ(input.scoring.mismatchScore, 2);
  EXPECT_EQ(input.scoring.gapScore, 3);
  EXPECT_EQ(input.first, "ABC");
  EXPECT_EQ(input.second, "DEF");
}

TEST(InputParserParseAlignmentInputTest, AllowsEmptySequences) {
  const std::vector<std::string> lines = {
      "1 2 3",
      "",
      "",
  };

  const AlignmentInput input = parseAlignmentInput(lines);

  EXPECT_EQ(input.scoring.matchScore, 1);
  EXPECT_EQ(input.scoring.mismatchScore, 2);
  EXPECT_EQ(input.scoring.gapScore, 3);
  EXPECT_EQ(input.first, "");
  EXPECT_EQ(input.second, "");
}

TEST(InputParserParseAlignmentInputTest, ThrowsWhenNoLines) {
  EXPECT_THROW(parseAlignmentInput({}), InputFormatError);
}

TEST(InputParserParseAlignmentInputTest, ThrowsWhenTooFewLines) {
  EXPECT_THROW(parseAlignmentInput({"1 2 3", "ABC"}), InputFormatError);
}

TEST(InputParserParseAlignmentInputTest, ThrowsWhenTooManyLines) {
  EXPECT_THROW(parseAlignmentInput({"1 2 3", "ABC", "DEF", "extra"}),
               InputFormatError);
}

TEST(InputParserParseAlignmentInputTest, ThrowsWhenFirstLineEmpty) {
  EXPECT_THROW(parseAlignmentInput({"", "ABC", "DEF"}), InputFormatError);
}

TEST(InputParserParseAlignmentInputTest, ThrowsWhenFirstLineHasOnlyOneInteger) {
  EXPECT_THROW(parseAlignmentInput({"1", "ABC", "DEF"}), InputFormatError);
}

TEST(InputParserParseAlignmentInputTest,
     ThrowsWhenFirstLineHasOnlyTwoIntegers) {
  EXPECT_THROW(parseAlignmentInput({"1 2", "ABC", "DEF"}), InputFormatError);
}

TEST(InputParserParseAlignmentInputTest,
     ThrowsWhenFirstLineHasNonIntegerToken) {
  EXPECT_THROW(parseAlignmentInput({"1 x 3", "ABC", "DEF"}), InputFormatError);
}

TEST(InputParserParseAlignmentInputTest, ThrowsWhenFirstLineHasFloatingPoint) {
  EXPECT_THROW(parseAlignmentInput({"1 2.5 3", "ABC", "DEF"}),
               InputFormatError);
}

TEST(InputParserParseAlignmentInputTest, SupportsIntMinAndMax) {
  const std::vector<std::string> lines = {
      std::to_string(std::numeric_limits<int>::min()) + " " +
          std::to_string(std::numeric_limits<int>::max()) + " 0",
      "A",
      "B",
  };

  const AlignmentInput input = parseAlignmentInput(lines);

  EXPECT_EQ(input.scoring.matchScore, std::numeric_limits<int>::min());
  EXPECT_EQ(input.scoring.mismatchScore, std::numeric_limits<int>::max());
  EXPECT_EQ(input.scoring.gapScore, 0);
}

TEST(InputParserParseExactSearchInputTest, ValidInput) {
  const ExactSearchInput input = parseExactSearchInput("hello world", "world");

  EXPECT_EQ(input.text, "hello world");
  EXPECT_EQ(input.pattern, "world");
}

TEST(InputParserParseExactSearchInputTest, TextLengthOnePatternLengthOne) {
  const ExactSearchInput input = parseExactSearchInput("a", "a");

  EXPECT_EQ(input.text, "a");
  EXPECT_EQ(input.pattern, "a");
}

TEST(InputParserParseExactSearchInputTest, PatternCanEqualTextLength) {
  const ExactSearchInput input = parseExactSearchInput("abc", "abc");

  EXPECT_EQ(input.text, "abc");
  EXPECT_EQ(input.pattern, "abc");
}

TEST(InputParserParseExactSearchInputTest, AllowsMaxTextLength) {
  const std::string text(10000, 'a');
  const std::string pattern(100, 'a');

  const ExactSearchInput input = parseExactSearchInput(text, pattern);

  EXPECT_EQ(input.text.size(), size_t{10000});
  EXPECT_EQ(input.pattern.size(), size_t{100});
}

TEST(InputParserParseExactSearchInputTest,
     AllowsMaxPatternLengthWhenTextSameSize) {
  const std::string text(100, 'a');
  const std::string pattern(100, 'a');

  const ExactSearchInput input = parseExactSearchInput(text, pattern);

  EXPECT_EQ(input.text.size(), size_t{100});
  EXPECT_EQ(input.pattern.size(), size_t{100});
}

TEST(InputParserParseExactSearchInputTest, ThrowsWhenTextEmpty) {
  EXPECT_THROW(parseExactSearchInput("", "a"), InputFormatError);
}

TEST(InputParserParseExactSearchInputTest, ThrowsWhenPatternEmpty) {
  EXPECT_THROW(parseExactSearchInput("abc", ""), InputFormatError);
}

TEST(InputParserParseExactSearchInputTest, ThrowsWhenBothEmpty) {
  EXPECT_THROW(parseExactSearchInput("", ""), InputFormatError);
}

TEST(InputParserParseExactSearchInputTest, ThrowsWhenTextTooLong) {
  const std::string text(10001, 'a');

  EXPECT_THROW(parseExactSearchInput(text, "a"), InputFormatError);
}

TEST(InputParserParseExactSearchInputTest, ThrowsWhenPatternTooLong) {
  const std::string text(200, 'a');
  const std::string pattern(101, 'a');

  EXPECT_THROW(parseExactSearchInput(text, pattern), InputFormatError);
}

TEST(InputParserParseExactSearchInputTest, ThrowsWhenPatternLongerThanText) {
  EXPECT_THROW(parseExactSearchInput("ab", "abc"), InputFormatError);
}

TEST(InputParserParseExactSearchInputTest,
     ThrowsWhenPatternLongerThanTextEvenIfPatternWithinLimit) {
  const std::string text(99, 'a');
  const std::string pattern(100, 'a');

  EXPECT_THROW(parseExactSearchInput(text, pattern), InputFormatError);
}

TEST(InputParserParseKSimilarityInputTest, ValidInput) {
  const KSimilarityInput input = parseKSimilarityInput({"abc", "bca"});

  EXPECT_EQ(input.source, "abc");
  EXPECT_EQ(input.target, "bca");
}

TEST(InputParserParseKSimilarityInputTest, AllowsEmptyStrings) {
  const KSimilarityInput input = parseKSimilarityInput({"", ""});

  EXPECT_EQ(input.source, "");
  EXPECT_EQ(input.target, "");
}

TEST(InputParserParseKSimilarityInputTest, ThrowsWhenNoLines) {
  EXPECT_THROW(parseKSimilarityInput({}), InputFormatError);
}

TEST(InputParserParseKSimilarityInputTest, ThrowsWhenOneLine) {
  EXPECT_THROW(parseKSimilarityInput({"abc"}), InputFormatError);
}

TEST(InputParserParseKSimilarityInputTest, ThrowsWhenTooManyLines) {
  EXPECT_THROW(parseKSimilarityInput({"abc", "bca", "extra"}),
               InputFormatError);
}

TEST(InputParserParseMinWindowInputTest, ValidInput) {
  const MinWindowInput input = parseMinWindowInput({"ADOBECODEBANC", "ABC"});

  EXPECT_EQ(input.source, "ADOBECODEBANC");
  EXPECT_EQ(input.target, "ABC");
}

TEST(InputParserParseMinWindowInputTest, AllowsEmptyStrings) {
  const MinWindowInput input = parseMinWindowInput({"", ""});

  EXPECT_EQ(input.source, "");
  EXPECT_EQ(input.target, "");
}

TEST(InputParserParseMinWindowInputTest, ThrowsWhenNoLines) {
  EXPECT_THROW(parseMinWindowInput({}), InputFormatError);
}

TEST(InputParserParseMinWindowInputTest, ThrowsWhenOneLine) {
  EXPECT_THROW(parseMinWindowInput({"abc"}), InputFormatError);
}

TEST(InputParserParseMinWindowInputTest, ThrowsWhenTooManyLines) {
  EXPECT_THROW(parseMinWindowInput({"abc", "a", "extra"}), InputFormatError);
}

TEST(InputParserParseRegexMatchInputTest, ValidInput) {
  const RegexMatchInput input = parseRegexMatchInput({"aa", "a*"});

  EXPECT_EQ(input.sequence, "aa");
  EXPECT_EQ(input.pattern, "a*");
}

TEST(InputParserParseRegexMatchInputTest, AllowsEmptyStrings) {
  const RegexMatchInput input = parseRegexMatchInput({"", ""});

  EXPECT_EQ(input.sequence, "");
  EXPECT_EQ(input.pattern, "");
}

TEST(InputParserParseRegexMatchInputTest, PreservesSpacesInTextAndPattern) {
  const RegexMatchInput input = parseRegexMatchInput({"a b c", "a.*c"});

  EXPECT_EQ(input.sequence, "a b c");
  EXPECT_EQ(input.pattern, "a.*c");
}

TEST(InputParserParseRegexMatchInputTest, ThrowsWhenNoLines) {
  EXPECT_THROW(parseRegexMatchInput({}), InputFormatError);
}

TEST(InputParserParseRegexMatchInputTest, ThrowsWhenOneLine) {
  EXPECT_THROW(parseRegexMatchInput({"abc"}), InputFormatError);
}

TEST(InputParserParseRegexMatchInputTest, ThrowsWhenTooManyLines) {
  EXPECT_THROW(parseRegexMatchInput({"abc", "a.*", "extra"}), InputFormatError);
}