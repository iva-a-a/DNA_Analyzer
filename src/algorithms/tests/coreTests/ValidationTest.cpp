#include <gtest/gtest.h>

#include "../../core/common/validation/Validation.hpp"

TEST(ValidationTest, IsDnaCharReturnsTrueForValidCharacters) {
  EXPECT_TRUE(Validation::isDnaChar('A'));
  EXPECT_TRUE(Validation::isDnaChar('C'));
  EXPECT_TRUE(Validation::isDnaChar('G'));
  EXPECT_TRUE(Validation::isDnaChar('T'));
}

TEST(ValidationTest, IsDnaCharReturnsFalseForLowercaseCharacters) {
  EXPECT_FALSE(Validation::isDnaChar('a'));
  EXPECT_FALSE(Validation::isDnaChar('c'));
  EXPECT_FALSE(Validation::isDnaChar('g'));
  EXPECT_FALSE(Validation::isDnaChar('t'));
}

TEST(ValidationTest, IsDnaCharReturnsFalseForInvalidLetters) {
  EXPECT_FALSE(Validation::isDnaChar('B'));
  EXPECT_FALSE(Validation::isDnaChar('D'));
  EXPECT_FALSE(Validation::isDnaChar('N'));
  EXPECT_FALSE(Validation::isDnaChar('X'));
}

TEST(ValidationTest, IsDnaCharReturnsFalseForDigits) {
  EXPECT_FALSE(Validation::isDnaChar('0'));
  EXPECT_FALSE(Validation::isDnaChar('1'));
  EXPECT_FALSE(Validation::isDnaChar('9'));
}

TEST(ValidationTest, IsDnaCharReturnsFalseForWhitespace) {
  EXPECT_FALSE(Validation::isDnaChar(' '));
  EXPECT_FALSE(Validation::isDnaChar('\n'));
  EXPECT_FALSE(Validation::isDnaChar('\t'));
}

TEST(ValidationTest, IsDnaCharReturnsFalseForSpecialCharacters) {
  EXPECT_FALSE(Validation::isDnaChar('.'));
  EXPECT_FALSE(Validation::isDnaChar('*'));
  EXPECT_FALSE(Validation::isDnaChar('?'));
  EXPECT_FALSE(Validation::isDnaChar('+'));
  EXPECT_FALSE(Validation::isDnaChar('-'));
  EXPECT_FALSE(Validation::isDnaChar('!'));
}

TEST(ValidationTest, IsDnaStringReturnsTrueForEmptyString) {
  EXPECT_TRUE(Validation::isDnaString(""));
}

TEST(ValidationTest, IsDnaStringReturnsTrueForSingleValidCharacters) {
  EXPECT_TRUE(Validation::isDnaString("A"));
  EXPECT_TRUE(Validation::isDnaString("C"));
  EXPECT_TRUE(Validation::isDnaString("G"));
  EXPECT_TRUE(Validation::isDnaString("T"));
}

TEST(ValidationTest, IsDnaStringReturnsTrueForValidDnaStrings) {
  EXPECT_TRUE(Validation::isDnaString("ACGT"));
  EXPECT_TRUE(Validation::isDnaString("AAAA"));
  EXPECT_TRUE(Validation::isDnaString("CCCC"));
  EXPECT_TRUE(Validation::isDnaString("GGGG"));
  EXPECT_TRUE(Validation::isDnaString("TTTT"));
  EXPECT_TRUE(Validation::isDnaString("ACGTACGTACGT"));
}

TEST(ValidationTest, IsDnaStringReturnsFalseForLowercaseCharacters) {
  EXPECT_FALSE(Validation::isDnaString("a"));
  EXPECT_FALSE(Validation::isDnaString("acgt"));
  EXPECT_FALSE(Validation::isDnaString("ACGt"));
}

TEST(ValidationTest, IsDnaStringReturnsFalseForInvalidCharacters) {
  EXPECT_FALSE(Validation::isDnaString("X"));
  EXPECT_FALSE(Validation::isDnaString("ACGX"));
  EXPECT_FALSE(Validation::isDnaString("1234"));
  EXPECT_FALSE(Validation::isDnaString("ACGT!"));
  EXPECT_FALSE(Validation::isDnaString("A C G T"));
  EXPECT_FALSE(Validation::isDnaString("A\nCGT"));
  EXPECT_FALSE(Validation::isDnaString("A\tCGT"));
}

TEST(ValidationTest, IsDnaStringReturnsFalseWhenInvalidCharacterIsAtBeginning) {
  EXPECT_FALSE(Validation::isDnaString("XACGT"));
}

TEST(ValidationTest, IsDnaStringReturnsFalseWhenInvalidCharacterIsInMiddle) {
  EXPECT_FALSE(Validation::isDnaString("ACXGT"));
}

TEST(ValidationTest, IsDnaStringReturnsFalseWhenInvalidCharacterIsAtEnd) {
  EXPECT_FALSE(Validation::isDnaString("ACGTX"));
}