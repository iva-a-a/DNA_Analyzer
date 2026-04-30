#include <gtest/gtest.h>

#include "../core/common/dnaSequence/DnaSequence.hpp"
#include "../core/common/errors/Errors.hpp"

TEST(DnaSequenceTest, DefaultConstructorCreatesEmptySequence) {
  DnaSequence sequence;

  EXPECT_TRUE(sequence.empty());
  EXPECT_EQ(sequence.size(), 0u);
  EXPECT_EQ(sequence.getSequence(), "");
}

TEST(DnaSequenceTest, ConstructorAcceptsEmptyString) {
  DnaSequence sequence("");

  EXPECT_TRUE(sequence.empty());
  EXPECT_EQ(sequence.size(), 0u);
  EXPECT_EQ(sequence.getSequence(), "");
}

TEST(DnaSequenceTest, ConstructorAcceptsValidDnaSequence) {
  DnaSequence sequence("ACGT");

  EXPECT_FALSE(sequence.empty());
  EXPECT_EQ(sequence.size(), 4u);
  EXPECT_EQ(sequence.getSequence(), "ACGT");
}

TEST(DnaSequenceTest, ConstructorAcceptsSingleCharacterSequences) {
  EXPECT_NO_THROW(DnaSequence("A"));
  EXPECT_NO_THROW(DnaSequence("C"));
  EXPECT_NO_THROW(DnaSequence("G"));
  EXPECT_NO_THROW(DnaSequence("T"));
}

TEST(DnaSequenceTest, ConstructorAcceptsLongValidSequence) {
  DnaSequence sequence("ACGTACGTACGT");

  EXPECT_EQ(sequence.size(), 12u);
  EXPECT_EQ(sequence.getSequence(), "ACGTACGTACGT");
}

TEST(DnaSequenceTest, ConstructorThrowsValidationErrorForInvalidCharacters) {
  EXPECT_THROW(DnaSequence("ACGX"), ValidationError);
  EXPECT_THROW(DnaSequence("1234"), ValidationError);
  EXPECT_THROW(DnaSequence("AC GT"), ValidationError);
  EXPECT_THROW(DnaSequence("acgt"), ValidationError);
  EXPECT_THROW(DnaSequence("ACGT!"), ValidationError);
}

TEST(DnaSequenceTest, ConstructorThrowsValidationErrorWithCorrectMessage) {
  try {
    DnaSequence sequence("ACGX");
    FAIL() << "Expected ValidationError";
  } catch (const ValidationError &error) {
    EXPECT_STREQ(error.what(), "DnaSequence contains invalid characters");
  }
}

TEST(DnaSequenceTest, GetSequenceReturnsConstReferenceToInternalString) {
  DnaSequence sequence("ACGT");

  const std::string &storedSequence = sequence.getSequence();

  EXPECT_EQ(storedSequence, "ACGT");
  EXPECT_EQ(&storedSequence, &sequence.getSequence());
}