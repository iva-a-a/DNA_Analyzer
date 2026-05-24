#include <gtest/gtest.h>

#include "../../data/DnaRecord.hpp"

TEST(DnaRecordTest, InitializesFieldsWithAggregateInitialization) {
  DnaRecord record{"ACGT", 2, "human"};

  EXPECT_EQ(record.sequence, "ACGT");
  EXPECT_EQ(record.classId, 2);
  EXPECT_EQ(record.species, "human");
}

TEST(DnaRecordTest, DefaultConstructedRecordHasDefaultValues) {
  DnaRecord record;

  EXPECT_TRUE(record.sequence.empty());
  EXPECT_TRUE(record.species.empty());
}

TEST(DnaRecordTest, FieldsCanBeModifiedAfterConstruction) {
  DnaRecord record;

  record.sequence = "TGCA";
  record.classId = 5;
  record.species = "dog";

  EXPECT_EQ(record.sequence, "TGCA");
  EXPECT_EQ(record.classId, 5);
  EXPECT_EQ(record.species, "dog");
}

TEST(DnaRecordTest, CanBeCopied) {
  DnaRecord original{"AAAA", 1, "chimp"};

  DnaRecord copy = original;

  EXPECT_EQ(copy.sequence, original.sequence);
  EXPECT_EQ(copy.classId, original.classId);
  EXPECT_EQ(copy.species, original.species);
}

TEST(DnaRecordTest, CopyIsIndependentFromOriginal) {
  DnaRecord original{"CCCC", 3, "cat"};

  DnaRecord copy = original;
  copy.sequence = "GGGG";
  copy.classId = 4;
  copy.species = "dog";

  EXPECT_EQ(original.sequence, "CCCC");
  EXPECT_EQ(original.classId, 3);
  EXPECT_EQ(original.species, "cat");

  EXPECT_EQ(copy.sequence, "GGGG");
  EXPECT_EQ(copy.classId, 4);
  EXPECT_EQ(copy.species, "dog");
}