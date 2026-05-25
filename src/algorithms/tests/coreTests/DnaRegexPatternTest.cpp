#include <gtest/gtest.h>

#include "../../core/common/errors/Errors.hpp"
#include "../../core/regex/models/DnaRegexPattern.hpp"

TEST(DnaRegexPatternTest, AcceptsDnaLetters) {
  const DnaRegexPattern pattern("ACGT");

  EXPECT_EQ(pattern.value(), "ACGT");
}

TEST(DnaRegexPatternTest, AcceptsDnaLettersAndRegexOperators) {
  const DnaRegexPattern pattern("A.C*G?T+");

  EXPECT_EQ(pattern.value(), "A.C*G?T+");
}

TEST(DnaRegexPatternTest, ThrowsWhenEmpty) {
  EXPECT_THROW(DnaRegexPattern(""), ValidationError);
}

TEST(DnaRegexPatternTest, ThrowsWhenContainsNonDnaLetter) {
  EXPECT_THROW(DnaRegexPattern("ABGT"), ValidationError);
}

TEST(DnaRegexPatternTest, ThrowsWhenContainsInvalidSymbol) {
  EXPECT_THROW(DnaRegexPattern("ACGT-"), ValidationError);
}