#include <gtest/gtest.h>

#include "../../core/common/errors/Errors.hpp"
#include "../../core/regex/parser/PatternParser.hpp"

class PatternParserTest : public ::testing::Test {
protected:
  PatternParser parser;
};

TEST_F(PatternParserTest, ParsesSingleLiteral) {
  auto tokens = parser.parse("A");

  ASSERT_EQ(tokens.size(), size_t{1});
  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  ASSERT_TRUE(tokens[0].value.has_value());
  EXPECT_EQ(tokens[0].value.value(), 'A');
}

TEST_F(PatternParserTest, ParsesAllDnaLiterals) {
  auto tokens = parser.parse("ACGT");

  ASSERT_EQ(tokens.size(), size_t{4});

  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'A');

  EXPECT_EQ(tokens[1].type, TokenType::Literal);
  EXPECT_EQ(tokens[1].value.value(), 'C');

  EXPECT_EQ(tokens[2].type, TokenType::Literal);
  EXPECT_EQ(tokens[2].value.value(), 'G');

  EXPECT_EQ(tokens[3].type, TokenType::Literal);
  EXPECT_EQ(tokens[3].value.value(), 'T');
}

TEST_F(PatternParserTest, ParsesAnySingle) {
  auto tokens = parser.parse(".");

  ASSERT_EQ(tokens.size(), size_t{1});
  EXPECT_EQ(tokens[0].type, TokenType::AnySingle);
  EXPECT_FALSE(tokens[0].value.has_value());
}

TEST_F(PatternParserTest, ParsesAnyOptional) {
  auto tokens = parser.parse("?");

  ASSERT_EQ(tokens.size(), size_t{1});
  EXPECT_EQ(tokens[0].type, TokenType::AnyOptional);
  EXPECT_FALSE(tokens[0].value.has_value());
}

TEST_F(PatternParserTest, ParsesAnySequence) {
  auto tokens = parser.parse("*");

  ASSERT_EQ(tokens.size(), size_t{1});
  EXPECT_EQ(tokens[0].type, TokenType::AnySequence);
  EXPECT_FALSE(tokens[0].value.has_value());
}

TEST_F(PatternParserTest, ParsesRepeatPrevious) {
  auto tokens = parser.parse("A+");

  ASSERT_EQ(tokens.size(), size_t{2});

  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'A');

  EXPECT_EQ(tokens[1].type, TokenType::RepeatPrevious);
  EXPECT_FALSE(tokens[1].value.has_value());
}

TEST_F(PatternParserTest, ParsesMixedPattern) {
  auto tokens = parser.parse("G?G*AC+A*A.");

  ASSERT_EQ(tokens.size(), size_t{11});

  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[1].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[2].type, TokenType::Literal);
  EXPECT_EQ(tokens[3].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[4].type, TokenType::Literal);
  EXPECT_EQ(tokens[5].type, TokenType::Literal);
  EXPECT_EQ(tokens[6].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[7].type, TokenType::Literal);
  EXPECT_EQ(tokens[8].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[9].type, TokenType::Literal);
  EXPECT_EQ(tokens[10].type, TokenType::AnySingle);
}

TEST_F(PatternParserTest, ThrowsOnEmptyPattern) {
  EXPECT_THROW(parser.parse(""), ValidationError);
}

TEST_F(PatternParserTest, ThrowsOnInvalidCharacterDigit) {
  EXPECT_THROW(parser.parse("A1C"), ValidationError);
}

TEST_F(PatternParserTest, ThrowsOnInvalidCharacterSymbol) {
  EXPECT_THROW(parser.parse("A-C"), ValidationError);
}

TEST_F(PatternParserTest, ThrowsWhenPlusIsFirstCharacter) {
  EXPECT_THROW(parser.parse("+A"), ValidationError);
}

TEST_F(PatternParserTest, ThrowsWhenPlusFollowsStar) {
  EXPECT_THROW(parser.parse("*+"), ValidationError);
}

TEST_F(PatternParserTest, ThrowsWhenPlusFollowsQuestion) {
  EXPECT_THROW(parser.parse("?+"), ValidationError);
}

TEST_F(PatternParserTest, ThrowsWhenPlusFollowsPlus) {
  EXPECT_THROW(parser.parse("A++"), ValidationError);
}

TEST_F(PatternParserTest, AllowsPlusAfterLiteral) {
  EXPECT_NO_THROW(parser.parse("A+"));
}

TEST_F(PatternParserTest, AllowsPlusAfterDot) {
  EXPECT_NO_THROW(parser.parse(".+"));
}

TEST_F(PatternParserTest, ParsesLiteralWithStar) {
  auto tokens = parser.parse("A*");

  ASSERT_EQ(tokens.size(), size_t{2});
  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'A');
  EXPECT_EQ(tokens[1].type, TokenType::AnySequence);
  EXPECT_FALSE(tokens[1].value.has_value());
}

TEST_F(PatternParserTest, ParsesLiteralWithQuestion) {
  auto tokens = parser.parse("A?");

  ASSERT_EQ(tokens.size(), size_t{2});
  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'A');
  EXPECT_EQ(tokens[1].type, TokenType::AnyOptional);
  EXPECT_FALSE(tokens[1].value.has_value());
}

TEST_F(PatternParserTest, ParsesDotWithStar) {
  auto tokens = parser.parse(".*");

  ASSERT_EQ(tokens.size(), size_t{2});
  EXPECT_EQ(tokens[0].type, TokenType::AnySingle);
  EXPECT_EQ(tokens[1].type, TokenType::AnySequence);
}

TEST_F(PatternParserTest, ParsesDotWithQuestion) {
  auto tokens = parser.parse(".?");

  ASSERT_EQ(tokens.size(), size_t{2});
  EXPECT_EQ(tokens[0].type, TokenType::AnySingle);
  EXPECT_EQ(tokens[1].type, TokenType::AnyOptional);
}

TEST_F(PatternParserTest, ParsesMultiplePluses) {
  auto tokens = parser.parse("A+B+");

  ASSERT_EQ(tokens.size(), size_t{4});
  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'A');
  EXPECT_EQ(tokens[1].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[2].type, TokenType::Literal);
  EXPECT_EQ(tokens[2].value.value(), 'B');
  EXPECT_EQ(tokens[3].type, TokenType::RepeatPrevious);
}

TEST_F(PatternParserTest, ParsesStarFirst) {
  auto tokens = parser.parse("*A");

  ASSERT_EQ(tokens.size(), size_t{2});
  EXPECT_EQ(tokens[0].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[1].type, TokenType::Literal);
  EXPECT_EQ(tokens[1].value.value(), 'A');
}

TEST_F(PatternParserTest, ParsesQuestionFirst) {
  auto tokens = parser.parse("?A");

  ASSERT_EQ(tokens.size(), size_t{2});
  EXPECT_EQ(tokens[0].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[1].type, TokenType::Literal);
  EXPECT_EQ(tokens[1].value.value(), 'A');
}

TEST_F(PatternParserTest, ParsesPlusThenStar) {
  auto tokens = parser.parse("A+*");

  ASSERT_EQ(tokens.size(), size_t{3});
  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'A');
  EXPECT_EQ(tokens[1].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[2].type, TokenType::AnySequence);
}

TEST_F(PatternParserTest, ParsesLowercaseLetters) {
  auto tokens = parser.parse("a");

  ASSERT_EQ(tokens.size(), size_t{1});
  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'a');
}

TEST_F(PatternParserTest, ThrowsOnInvalidCharacterSpace) {
  EXPECT_THROW(parser.parse("A C"), ValidationError);
}

TEST_F(PatternParserTest, ThrowsOnInvalidCharacterAt) {
  EXPECT_THROW(parser.parse("@"), ValidationError);
}

TEST_F(PatternParserTest, ThrowsOnInvalidCharacterHash) {
  EXPECT_THROW(parser.parse("#"), ValidationError);
}

TEST_F(PatternParserTest, ParsesLongComplexPattern1) {
  std::string pattern = "A?B*C+D.E?F*G+H.I*J+K?L.M*N+O.P*Q+R?S.T*U+V?W.X*Y+Z.";
  auto tokens = parser.parse(pattern);

  ASSERT_EQ(tokens.size(), size_t{52});

  EXPECT_EQ(tokens[0].type, TokenType::Literal);
  EXPECT_EQ(tokens[0].value.value(), 'A');
  EXPECT_EQ(tokens[1].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[2].type, TokenType::Literal);
  EXPECT_EQ(tokens[2].value.value(), 'B');
  EXPECT_EQ(tokens[3].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[4].type, TokenType::Literal);
  EXPECT_EQ(tokens[4].value.value(), 'C');
  EXPECT_EQ(tokens[5].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[6].type, TokenType::Literal);
  EXPECT_EQ(tokens[6].value.value(), 'D');
  EXPECT_EQ(tokens[7].type, TokenType::AnySingle);
  EXPECT_EQ(tokens[16].type, TokenType::Literal);
  EXPECT_EQ(tokens[16].value.value(), 'I');
  EXPECT_EQ(tokens[17].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[18].type, TokenType::Literal);
  EXPECT_EQ(tokens[18].value.value(), 'J');
  EXPECT_EQ(tokens[19].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[20].type, TokenType::Literal);
  EXPECT_EQ(tokens[20].value.value(), 'K');
  EXPECT_EQ(tokens[21].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[22].type, TokenType::Literal);
  EXPECT_EQ(tokens[22].value.value(), 'L');
  EXPECT_EQ(tokens[48].type, TokenType::Literal);
  EXPECT_EQ(tokens[48].value.value(), 'Y');
  EXPECT_EQ(tokens[49].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[50].type, TokenType::Literal);
  EXPECT_EQ(tokens[50].value.value(), 'Z');
  EXPECT_EQ(tokens[51].type, TokenType::AnySingle);
}

TEST_F(PatternParserTest, ParsesLongComplexPattern2) {
  std::string pattern = "**A??B+C..D?*E+F?*G.";
  auto tokens = parser.parse(pattern);

  ASSERT_EQ(tokens.size(), size_t{20});

  EXPECT_EQ(tokens[0].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[1].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[2].type, TokenType::Literal);
  EXPECT_EQ(tokens[2].value.value(), 'A');
  EXPECT_EQ(tokens[3].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[4].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[5].type, TokenType::Literal);
  EXPECT_EQ(tokens[5].value.value(), 'B');
  EXPECT_EQ(tokens[6].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[7].type, TokenType::Literal);
  EXPECT_EQ(tokens[7].value.value(), 'C');
  EXPECT_EQ(tokens[8].type, TokenType::AnySingle);
  EXPECT_EQ(tokens[9].type, TokenType::AnySingle);
  EXPECT_EQ(tokens[10].type, TokenType::Literal);
  EXPECT_EQ(tokens[10].value.value(), 'D');
  EXPECT_EQ(tokens[11].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[12].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[13].type, TokenType::Literal);
  EXPECT_EQ(tokens[13].value.value(), 'E');
  EXPECT_EQ(tokens[14].type, TokenType::RepeatPrevious);
  EXPECT_EQ(tokens[15].type, TokenType::Literal);
  EXPECT_EQ(tokens[15].value.value(), 'F');
  EXPECT_EQ(tokens[16].type, TokenType::AnyOptional);
  EXPECT_EQ(tokens[17].type, TokenType::AnySequence);
  EXPECT_EQ(tokens[18].type, TokenType::Literal);
  EXPECT_EQ(tokens[18].value.value(), 'G');
  EXPECT_EQ(tokens[19].type, TokenType::AnySingle);
}