#include <gtest/gtest.h>

#include "../../common/Errors.hpp"
#include "../../data/DnaDataset.hpp"
#include "../../data/validator/DatasetValidator.hpp"

namespace {

DnaRecord makeRecord(const std::string &sequence, int classId,
                     const std::string &species) {
  return DnaRecord{sequence, classId, species};
}

} // namespace

TEST(DatasetValidatorTest, AcceptsValidDataset) {
  DnaDataset dataset(
      {makeRecord("ACGT", 0, "human"), makeRecord("TGCA", 1, "dog")});

  EXPECT_NO_THROW(DatasetValidator::validate(dataset));
}

TEST(DatasetValidatorTest, ThrowsWhenDatasetIsEmpty) {
  DnaDataset dataset({});

  EXPECT_THROW(DatasetValidator::validate(dataset), ValidationError);
}

TEST(DatasetValidatorTest, ThrowsWhenSequenceIsEmpty) {
  DnaDataset dataset({makeRecord("", 0, "human")});

  try {
    DatasetValidator::validate(dataset);
    FAIL() << "Expected ValidationError";
  } catch (const ValidationError &error) {
    EXPECT_NE(std::string(error.what()).find("index 0"), std::string::npos);
    EXPECT_NE(std::string(error.what()).find("DNA sequence is empty"),
              std::string::npos);
  }
}

TEST(DatasetValidatorTest, ThrowsWhenSequenceHasInvalidCharacters) {
  DnaDataset dataset({makeRecord("ACGX", 0, "human")});

  try {
    DatasetValidator::validate(dataset);
    FAIL() << "Expected ValidationError";
  } catch (const ValidationError &error) {
    EXPECT_NE(std::string(error.what()).find("invalid characters"),
              std::string::npos);
    EXPECT_NE(std::string(error.what()).find("ACGX"), std::string::npos);
  }
}

TEST(DatasetValidatorTest, ThrowsWhenClassIdIsNegative) {
  DnaDataset dataset({makeRecord("ACGT", -1, "human")});

  try {
    DatasetValidator::validate(dataset);
    FAIL() << "Expected ValidationError";
  } catch (const ValidationError &error) {
    EXPECT_NE(std::string(error.what()).find("classId must be non-negative"),
              std::string::npos);
  }
}

TEST(DatasetValidatorTest, ThrowsWhenSpeciesIsEmpty) {
  DnaDataset dataset({makeRecord("ACGT", 0, "")});

  try {
    DatasetValidator::validate(dataset);
    FAIL() << "Expected ValidationError";
  } catch (const ValidationError &error) {
    EXPECT_NE(std::string(error.what()).find("species is empty"),
              std::string::npos);
  }
}

TEST(DatasetValidatorTest, ReportsCorrectIndexForSecondInvalidRecord) {
  DnaDataset dataset({makeRecord("ACGT", 0, "human"),
                      makeRecord("ACGT", 0, "dog"),
                      makeRecord("BAD!", 0, "cat")});

  try {
    DatasetValidator::validate(dataset);
    FAIL() << "Expected ValidationError";
  } catch (const ValidationError &error) {
    EXPECT_NE(std::string(error.what()).find("index 2"), std::string::npos);
  }
}

TEST(DatasetValidatorTest, AcceptsNInSequence) {
  DnaDataset dataset({makeRecord("ACGTN", 0, "human")});

  EXPECT_NO_THROW(DatasetValidator::validate(dataset));
}
