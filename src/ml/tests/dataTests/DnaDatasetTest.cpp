#include <gtest/gtest.h>

#include "../../data/DnaDataset.hpp"

namespace {

DnaRecord makeRecord(const std::string &sequence, int classId,
                     const std::string &species) {
  return DnaRecord{sequence, classId, species};
}

} // namespace

TEST(DnaDatasetTest, EmptyDatasetReportsZeroSize) {
  DnaDataset dataset({});

  EXPECT_TRUE(dataset.empty());
  EXPECT_EQ(dataset.size(), 0U);
}

TEST(DnaDatasetTest, SizeReturnsNumberOfRecords) {
  DnaDataset dataset(
      {makeRecord("ACGT", 0, "human"), makeRecord("TGCA", 1, "dog")});

  EXPECT_FALSE(dataset.empty());
  EXPECT_EQ(dataset.size(), 2U);
}

TEST(DnaDatasetTest, RecordsReturnsConstReferenceToInternalStorage) {
  std::vector<DnaRecord> records = {makeRecord("ACGT", 0, "human")};
  DnaDataset dataset(records);

  const std::vector<DnaRecord> &stored = dataset.records();

  ASSERT_EQ(stored.size(), 1U);
  EXPECT_EQ(stored[0].sequence, "ACGT");
  EXPECT_EQ(&stored, &dataset.records());
}

TEST(DnaDatasetTest, SequencesReturnsAllSequencesInOrder) {
  DnaDataset dataset({makeRecord("ACGT", 0, "human"),
                      makeRecord("TGCA", 1, "dog"),
                      makeRecord("AAAA", 0, "cat")});

  EXPECT_EQ(dataset.sequences(),
            (std::vector<std::string>{"ACGT", "TGCA", "AAAA"}));
}

TEST(DnaDatasetTest, ClassIdsReturnsAllClassIdsInOrder) {
  DnaDataset dataset({makeRecord("ACGT", 0, "human"),
                      makeRecord("TGCA", 1, "dog"),
                      makeRecord("AAAA", 0, "cat")});

  EXPECT_EQ(dataset.classIds(), (std::vector<int>{0, 1, 0}));
}

TEST(DnaDatasetTest, SpeciesReturnsAllSpeciesInOrder) {
  DnaDataset dataset({makeRecord("ACGT", 0, "human"),
                      makeRecord("TGCA", 1, "dog")});

  EXPECT_EQ(dataset.species(), (std::vector<std::string>{"human", "dog"}));
}

TEST(DnaDatasetTest, ClassCountReturnsNumberOfUniqueClasses) {
  DnaDataset dataset({makeRecord("ACGT", 0, "human"),
                      makeRecord("TGCA", 1, "dog"),
                      makeRecord("AAAA", 0, "human"),
                      makeRecord("CCCC", 2, "cat")});

  EXPECT_EQ(dataset.classCount(), 3);
}

TEST(DnaDatasetTest, ClassCountIsZeroForEmptyDataset) {
  DnaDataset dataset({});

  EXPECT_EQ(dataset.classCount(), 0);
}

TEST(DnaDatasetTest, ClassCountIsOneWhenAllRecordsShareClass) {
  DnaDataset dataset(
      {makeRecord("ACGT", 5, "a"), makeRecord("TGCA", 5, "b")});

  EXPECT_EQ(dataset.classCount(), 1);
}
