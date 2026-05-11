#include <gtest/gtest.h>

#include "../../core/common/errors/Errors.hpp"
#include "../../core/regex/RegexMatcher.hpp"

class RegexMatcherTest : public ::testing::Test {
protected:
  RegexMatcher matcher;
};

TEST_F(RegexMatcherTest, LiteralExactMatch) {
  EXPECT_TRUE(matcher.matches("ACGT", "ACGT"));
}

TEST_F(RegexMatcherTest, LiteralMismatch) {
  EXPECT_FALSE(matcher.matches("ACGT", "ACGA"));
}

TEST_F(RegexMatcherTest, LiteralPatternShorterThanText) {
  EXPECT_FALSE(matcher.matches("ACGT", "ACG"));
}

TEST_F(RegexMatcherTest, LiteralPatternLongerThanText) {
  EXPECT_FALSE(matcher.matches("ACG", "ACGT"));
}

TEST_F(RegexMatcherTest, DotMatchesSingleCharacter) {
  EXPECT_TRUE(matcher.matches("A", "."));
  EXPECT_TRUE(matcher.matches("C", "."));
  EXPECT_TRUE(matcher.matches("G", "."));
  EXPECT_TRUE(matcher.matches("T", "."));
}

TEST_F(RegexMatcherTest, DotDoesNotMatchEmptyText) {
  EXPECT_FALSE(matcher.matches("", "."));
}

TEST_F(RegexMatcherTest, DotInsidePattern) {
  EXPECT_TRUE(matcher.matches("ACG", "A.G"));
  EXPECT_FALSE(matcher.matches("AG", "A.G"));
}

TEST_F(RegexMatcherTest, QuestionMatchesZeroCharacters) {
  EXPECT_TRUE(matcher.matches("", "?"));
}

TEST_F(RegexMatcherTest, QuestionMatchesOneCharacter) {
  EXPECT_TRUE(matcher.matches("A", "?"));
}

TEST_F(RegexMatcherTest, QuestionDoesNotMatchTwoCharacters) {
  EXPECT_FALSE(matcher.matches("AC", "?"));
}

TEST_F(RegexMatcherTest, QuestionInsidePatternAsZeroCharacters) {
  EXPECT_TRUE(matcher.matches("AC", "A?C"));
}

TEST_F(RegexMatcherTest, QuestionInsidePatternAsOneCharacter) {
  EXPECT_TRUE(matcher.matches("AGC", "A?C"));
}

TEST_F(RegexMatcherTest, StarMatchesEmptyText) {
  EXPECT_TRUE(matcher.matches("", "*"));
}

TEST_F(RegexMatcherTest, StarMatchesAnySequence) {
  EXPECT_TRUE(matcher.matches("ACGTACGT", "*"));
}

TEST_F(RegexMatcherTest, StarInsidePatternMatchesEmptySequence) {
  EXPECT_TRUE(matcher.matches("AC", "A*C"));
}

TEST_F(RegexMatcherTest, StarInsidePatternMatchesLongSequence) {
  EXPECT_TRUE(matcher.matches("AGGGTTTC", "A*C"));
}

TEST_F(RegexMatcherTest, StarAtBeginning) {
  EXPECT_TRUE(matcher.matches("GGGAC", "*AC"));
}

TEST_F(RegexMatcherTest, StarAtEnd) {
  EXPECT_TRUE(matcher.matches("ACGGG", "AC*"));
}

TEST_F(RegexMatcherTest, PlusMatchesZeroPreviousLiterals) {
  EXPECT_FALSE(matcher.matches("", "A+"));
  EXPECT_FALSE(matcher.matches("C", "A+C"));
}

TEST_F(RegexMatcherTest, PlusMatchesOnePreviousLiteral) {
  EXPECT_TRUE(matcher.matches("A", "A+"));
}

TEST_F(RegexMatcherTest, PlusMatchesManyPreviousLiterals) {
  EXPECT_TRUE(matcher.matches("AAAA", "A+"));
}

TEST_F(RegexMatcherTest, PlusDoesNotMatchDifferentLiteral) {
  EXPECT_FALSE(matcher.matches("C", "A+"));
}

TEST_F(RegexMatcherTest, PlusInsidePattern) {
  EXPECT_FALSE(matcher.matches("AC", "AB+C"));
  EXPECT_TRUE(matcher.matches("ABC", "AB+C"));
  EXPECT_TRUE(matcher.matches("ABBBBC", "AB+C"));
  EXPECT_FALSE(matcher.matches("ABBBBD", "AB+C"));
}

TEST_F(RegexMatcherTest, PlusAfterDotMatchesAnyRepeatedCharacters) {
  EXPECT_FALSE(matcher.matches("", ".+"));
  EXPECT_TRUE(matcher.matches("A", ".+"));
  EXPECT_TRUE(matcher.matches("ACGT", ".+"));
}

TEST_F(RegexMatcherTest, CombinedPatternFromTaskExample) {
  EXPECT_TRUE(matcher.matches("GGCGACACCCACCATACAT", "G?G*AC+A*A."));
}

TEST_F(RegexMatcherTest, CombinedPatternNegativeCase) {
  EXPECT_FALSE(matcher.matches("TTTT", "G?G*AC+A*A."));
}

TEST_F(RegexMatcherTest, EmptyTextWithOptionalElements) {
  EXPECT_TRUE(matcher.matches("", "?"));
  EXPECT_TRUE(matcher.matches("", "*"));
  EXPECT_FALSE(matcher.matches("", "A+"));
  EXPECT_TRUE(matcher.matches("", "????"));
  EXPECT_TRUE(matcher.matches("", "***"));
}

TEST_F(RegexMatcherTest, EmptyTextWithRequiredLiteral) {
  EXPECT_FALSE(matcher.matches("", "A"));
}

TEST_F(RegexMatcherTest, EmptyTextWithRequiredDot) {
  EXPECT_FALSE(matcher.matches("", "."));
}

TEST_F(RegexMatcherTest, ComplexEmptyMatch) {
  EXPECT_FALSE(matcher.matches("", "A+B+C+"));
  EXPECT_FALSE(matcher.matches("", "?*A+"));
}

TEST_F(RegexMatcherTest, InvalidEmptyPatternThrows) {
  EXPECT_THROW(matcher.matches("ACGT", ""), ValidationError);
}

TEST_F(RegexMatcherTest, InvalidPatternCharacterThrows) {
  EXPECT_THROW(matcher.matches("ACGT", "A1C"), ValidationError);
}

TEST_F(RegexMatcherTest, InvalidPlusAtBeginningThrows) {
  EXPECT_THROW(matcher.matches("ACGT", "+A"), ValidationError);
}

TEST_F(RegexMatcherTest, InvalidPlusAfterStarThrows) {
  EXPECT_THROW(matcher.matches("ACGT", "*+"), ValidationError);
}

TEST_F(RegexMatcherTest, InvalidPlusAfterQuestionThrows) {
  EXPECT_THROW(matcher.matches("ACGT", "?+"), ValidationError);
}

TEST_F(RegexMatcherTest, InvalidPlusAfterPlusThrows) {
  EXPECT_THROW(matcher.matches("ACGT", "A++"), ValidationError);
}

TEST_F(RegexMatcherTest, MultipleStars) {
  EXPECT_TRUE(matcher.matches("", "**"));
  EXPECT_TRUE(matcher.matches("ACGT", "**"));
  EXPECT_TRUE(matcher.matches("ACGT", "A**T"));
}

TEST_F(RegexMatcherTest, MultipleQuestions) {
  EXPECT_TRUE(matcher.matches("", "??"));
  EXPECT_TRUE(matcher.matches("A", "??"));
  EXPECT_TRUE(matcher.matches("AC", "??"));
  EXPECT_FALSE(matcher.matches("ACG", "??"));
}

TEST_F(RegexMatcherTest, MixedOptionalAndRequired) {
  EXPECT_TRUE(matcher.matches("AC", "A??C"));
  EXPECT_TRUE(matcher.matches("AGC", "A??C"));
  EXPECT_TRUE(matcher.matches("AGTC", "A??C"));
  EXPECT_FALSE(matcher.matches("AGTTC", "A??C"));
}

TEST_F(RegexMatcherTest, FullTextMustBeMatched) {
  EXPECT_FALSE(matcher.matches("AAC", "AC"));
  EXPECT_FALSE(matcher.matches("ACA", "AC"));
}

TEST_F(RegexMatcherTest, StarCanBridgeRequiredParts) {
  EXPECT_TRUE(matcher.matches("AAACCCGGGT", "A*G*T"));
  EXPECT_FALSE(matcher.matches("AAACCCGGG", "A*G*T"));
}

TEST_F(RegexMatcherTest, RepeatPreviousAfterDotWithSuffix) {
  EXPECT_TRUE(matcher.matches("ACGT", ".+T"));
  EXPECT_FALSE(matcher.matches("T", ".+T"));
  EXPECT_FALSE(matcher.matches("ACGA", ".+T"));
}

TEST_F(RegexMatcherTest, StarOnlyPattern) {
  EXPECT_TRUE(matcher.matches("", "*"));
  EXPECT_TRUE(matcher.matches("A", "*"));
  EXPECT_TRUE(matcher.matches("AAA", "*"));
}

TEST_F(RegexMatcherTest, StarMatchesWholeString) {
  EXPECT_TRUE(matcher.matches("AA", "A*"));
  EXPECT_TRUE(matcher.matches("A", "A*"));
  EXPECT_FALSE(matcher.matches("", "A*"));
  EXPECT_FALSE(matcher.matches("B", "A*"));
}

TEST_F(RegexMatcherTest, PlusThenStar) {
  EXPECT_FALSE(matcher.matches("", "A+*"));
  EXPECT_TRUE(matcher.matches("A", "A+*"));
  EXPECT_TRUE(matcher.matches("AA", "A+*"));
  EXPECT_TRUE(matcher.matches("AB", "A+*"));
  EXPECT_TRUE(matcher.matches("AAB", "A+*"));
  EXPECT_FALSE(matcher.matches("BB", "A+*"));
  EXPECT_FALSE(matcher.matches("BB", "A+*"));
}

TEST_F(RegexMatcherTest, PlusThenQuestion) {
  EXPECT_FALSE(matcher.matches("", "A+?"));
  EXPECT_TRUE(matcher.matches("A", "A+?"));
  EXPECT_TRUE(matcher.matches("AA", "A+?"));
  EXPECT_TRUE(matcher.matches("AB", "A+?"));
  EXPECT_TRUE(matcher.matches("AAB", "A+?"));
  EXPECT_TRUE(matcher.matches("A", "A+?"));
  EXPECT_FALSE(matcher.matches("ABC", "A+?"));
}

TEST_F(RegexMatcherTest, StarThenQuestion) {
  EXPECT_TRUE(matcher.matches("", "*?"));
  EXPECT_TRUE(matcher.matches("A", "*?"));
  EXPECT_TRUE(matcher.matches("AB", "*?"));
  EXPECT_TRUE(matcher.matches("AB", "A*?B"));
  EXPECT_TRUE(matcher.matches("ACB", "A*?B"));
  EXPECT_TRUE(matcher.matches("AB", "A*?B"));
  EXPECT_TRUE(matcher.matches("ACDB", "A*?B"));
}

TEST_F(RegexMatcherTest, QuestionThenStar) {
  EXPECT_TRUE(matcher.matches("", "?*"));
  EXPECT_TRUE(matcher.matches("A", "?*"));
  EXPECT_TRUE(matcher.matches("ABC", "?*"));
  EXPECT_TRUE(matcher.matches("AB", "A?*B"));
  EXPECT_TRUE(matcher.matches("ACB", "A?*B"));
  EXPECT_TRUE(matcher.matches("ACDB", "A?*B"));
}

TEST_F(RegexMatcherTest, MixedOperatorsOnlyPattern) {
  EXPECT_TRUE(matcher.matches("", "?*?"));
  EXPECT_TRUE(matcher.matches("A", "?*?"));
  EXPECT_TRUE(matcher.matches("AB", "?*?"));
  EXPECT_TRUE(matcher.matches("ABC", "?*?"));
  EXPECT_TRUE(matcher.matches("ACGT", "?*?*?"));
}

TEST_F(RegexMatcherTest, PlusAtEndOfPattern) {
  EXPECT_TRUE(matcher.matches("AB", "AB+"));
  EXPECT_TRUE(matcher.matches("ABBB", "AB+"));
  EXPECT_FALSE(matcher.matches("A", "AB+"));
  EXPECT_FALSE(matcher.matches("AC", "AB+"));
}

TEST_F(RegexMatcherTest, DotPlusThenStar) {
  EXPECT_FALSE(matcher.matches("", ".+*"));
  EXPECT_TRUE(matcher.matches("A", ".+*"));
  EXPECT_TRUE(matcher.matches("ABC", ".+*"));
  EXPECT_TRUE(matcher.matches("XYZ", ".+*"));
}

TEST_F(RegexMatcherTest, DotPlusThenQuestion) {
  EXPECT_FALSE(matcher.matches("", ".+?"));
  EXPECT_TRUE(matcher.matches("A", ".+?"));
  EXPECT_TRUE(matcher.matches("AB", ".+?"));
  EXPECT_TRUE(matcher.matches("ABC", ".+?"));
}

TEST_F(RegexMatcherTest, ComplexSequenceWithPlusAndStar) {
  EXPECT_TRUE(matcher.matches("ABBBBCDD", "A+B*C?D+"));
  EXPECT_TRUE(matcher.matches("ABBBCD", "A+B*C?D+"));
  EXPECT_TRUE(matcher.matches("ABCD", "A+B*C?D+"));
  EXPECT_FALSE(matcher.matches("ACDD", "A+B*C?D+"));
}

TEST_F(RegexMatcherTest, EmptyTextWithStarQuestion) {
  EXPECT_TRUE(matcher.matches("", "*?"));
  EXPECT_TRUE(matcher.matches("", "?*"));
  EXPECT_TRUE(matcher.matches("", "?*?"));
  EXPECT_TRUE(matcher.matches("", "*?*"));
}