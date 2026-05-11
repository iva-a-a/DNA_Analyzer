#include <gtest/gtest.h>

#include "../../io/formatter/OutputFormatter.hpp"

using namespace OutputFormatter;

TEST(OutputFormatterFormatPositionsTest, EmptyPositionsReturnsEmptyString) {
  ExactSearchResult result;
  result.positions = {};

  EXPECT_EQ(formatPositions(result), "");
}

TEST(OutputFormatterFormatPositionsTest, SinglePosition) {
  ExactSearchResult result;
  result.positions = {5};

  EXPECT_EQ(formatPositions(result), "5");
}

TEST(OutputFormatterFormatPositionsTest, MultiplePositionsSeparatedBySpaces) {
  ExactSearchResult result;
  result.positions = {0, 3, 10, 42};

  EXPECT_EQ(formatPositions(result), "0 3 10 42");
}

TEST(OutputFormatterFormatPositionsTest, PositionsHaveNoTrailingSpace) {
  ExactSearchResult result;
  result.positions = {1, 2, 3};

  const std::string output = formatPositions(result);

  ASSERT_FALSE(output.empty());
  EXPECT_NE(output.back(), ' ');
}

TEST(OutputFormatterFormatPositionsTest, LargePositionValue) {
  ExactSearchResult result;
  result.positions = {std::numeric_limits<std::size_t>::max()};

  EXPECT_EQ(formatPositions(result),
            std::to_string(std::numeric_limits<std::size_t>::max()));
}

TEST(OutputFormatterFormatIntTest, Zero) { EXPECT_EQ(formatInt(0), "0"); }

TEST(OutputFormatterFormatIntTest, PositiveValue) {
  EXPECT_EQ(formatInt(123), "123");
}

TEST(OutputFormatterFormatIntTest, NegativeValue) {
  EXPECT_EQ(formatInt(-123), "-123");
}

TEST(OutputFormatterFormatIntTest, MinInt) {
  EXPECT_EQ(formatInt(std::numeric_limits<int>::min()),
            std::to_string(std::numeric_limits<int>::min()));
}

TEST(OutputFormatterFormatIntTest, MaxInt) {
  EXPECT_EQ(formatInt(std::numeric_limits<int>::max()),
            std::to_string(std::numeric_limits<int>::max()));
}

TEST(OutputFormatterFormatAlignmentTest, BasicAlignment) {
  AlignmentResult result;
  result.score = 10;
  result.alignedFirst = "ACGT";
  result.matchLine = "||||";
  result.alignedSecond = "ACGT";

  EXPECT_EQ(formatAlignment(result), "10\nACGT\n||||\nACGT");
}

TEST(OutputFormatterFormatAlignmentTest, NegativeScore) {
  AlignmentResult result;
  result.score = -5;
  result.alignedFirst = "A-C";
  result.matchLine = "| |";
  result.alignedSecond = "ATC";

  EXPECT_EQ(formatAlignment(result), "-5\nA-C\n| |\nATC");
}

TEST(OutputFormatterFormatAlignmentTest, ZeroScore) {
  AlignmentResult result;
  result.score = 0;
  result.alignedFirst = "A";
  result.matchLine = " ";
  result.alignedSecond = "G";

  EXPECT_EQ(formatAlignment(result), "0\nA\n \nG");
}

TEST(OutputFormatterFormatAlignmentTest, EmptyAlignmentStrings) {
  AlignmentResult result;
  result.score = 0;
  result.alignedFirst = "";
  result.matchLine = "";
  result.alignedSecond = "";

  EXPECT_EQ(formatAlignment(result), "0\n\n\n");
}

TEST(OutputFormatterFormatAlignmentTest, NoExtraNewlineAtEnd) {
  AlignmentResult result;
  result.score = 1;
  result.alignedFirst = "A";
  result.matchLine = "|";
  result.alignedSecond = "A";

  const std::string output = formatAlignment(result);

  EXPECT_EQ(output, "1\nA\n|\nA");
  EXPECT_NE(output.back(), '\n');
}

TEST(OutputFormatterFormatAlignmentTest, PreservesSpacesAndDashes) {
  AlignmentResult result;
  result.score = 7;
  result.alignedFirst = "A--CG T";
  result.matchLine = "|  || |";
  result.alignedSecond = "ATT CGT";

  EXPECT_EQ(formatAlignment(result), "7\nA--CG T\n|  || |\nATT CGT");
}

TEST(OutputFormatterFormatBooleanTest, TrueReturnsCapitalizedTrue) {
  EXPECT_EQ(formatBoolean(true), "True");
}

TEST(OutputFormatterFormatBooleanTest, FalseReturnsCapitalizedFalse) {
  EXPECT_EQ(formatBoolean(false), "False");
}