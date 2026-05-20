#include <gtest/gtest.h>

#include "../../utils/DnaValidation.hpp"

TEST(DnaValidationTest, EmptyStringIsValid) {
  EXPECT_TRUE(DnaValidation::isDnaSequence(""));
}

TEST(DnaValidationTest, AcceptsAllValidNucleotides) {
  EXPECT_TRUE(DnaValidation::isDnaSequence("ACGT"));
  EXPECT_TRUE(DnaValidation::isDnaSequence("A"));
  EXPECT_TRUE(DnaValidation::isDnaSequence("C"));
  EXPECT_TRUE(DnaValidation::isDnaSequence("G"));
  EXPECT_TRUE(DnaValidation::isDnaSequence("T"));
  EXPECT_TRUE(DnaValidation::isDnaSequence("N"));
}

TEST(DnaValidationTest, AcceptsLongValidSequence) {
  EXPECT_TRUE(DnaValidation::isDnaSequence("ACGTACGTNNNNACGT"));
}

TEST(DnaValidationTest, RejectsLowercaseLetters) {
  EXPECT_FALSE(DnaValidation::isDnaSequence("acgt"));
  EXPECT_FALSE(DnaValidation::isDnaSequence("Acgt"));
}

TEST(DnaValidationTest, RejectsInvalidCharacters) {
  EXPECT_FALSE(DnaValidation::isDnaSequence("ACGX"));
  EXPECT_FALSE(DnaValidation::isDnaSequence("1234"));
  EXPECT_FALSE(DnaValidation::isDnaSequence("AC GT"));
  EXPECT_FALSE(DnaValidation::isDnaSequence("ACGT!"));
  EXPECT_FALSE(DnaValidation::isDnaSequence("U"));
}

TEST(DnaValidationTest, RejectsMixedValidAndInvalidCharacters) {
  EXPECT_FALSE(DnaValidation::isDnaSequence("ACGTAXGT"));
}
