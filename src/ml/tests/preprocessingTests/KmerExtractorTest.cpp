#include <gtest/gtest.h>

#include "../../common/Errors.hpp"
#include "../../preprocessing/KmerExtractor.hpp"

TEST(KmerExtractorTest, ExtractsAllKmersForValidInput) {
  std::vector<std::string> kmers =
      KmerExtractor::extractKmers("ACGTAC", 2);

  ASSERT_EQ(kmers.size(), 5U);
  EXPECT_EQ(kmers[0], "AC");
  EXPECT_EQ(kmers[1], "CG");
  EXPECT_EQ(kmers[2], "GT");
  EXPECT_EQ(kmers[3], "TA");
  EXPECT_EQ(kmers[4], "AC");
}

TEST(KmerExtractorTest, ExtractsSingleKmerWhenKEqualsSequenceLength) {
  std::vector<std::string> kmers = KmerExtractor::extractKmers("ACGT", 4);

  ASSERT_EQ(kmers.size(), 1U);
  EXPECT_EQ(kmers[0], "ACGT");
}

TEST(KmerExtractorTest, ReturnsEmptyVectorWhenKGreaterThanSequenceLength) {
  std::vector<std::string> kmers = KmerExtractor::extractKmers("AC", 3);

  EXPECT_TRUE(kmers.empty());
}

TEST(KmerExtractorTest, ReturnsEmptyVectorForEmptySequence) {
  std::vector<std::string> kmers = KmerExtractor::extractKmers("", 1);

  EXPECT_TRUE(kmers.empty());
}

TEST(KmerExtractorTest, ThrowsValidationErrorWhenKIsZero) {
  EXPECT_THROW(KmerExtractor::extractKmers("ACGT", 0), ValidationError);
}

TEST(KmerExtractorTest, Extracts1MersFromSequence) {
  std::vector<std::string> kmers = KmerExtractor::extractKmers("AC", 1);

  ASSERT_EQ(kmers.size(), 2U);
  EXPECT_EQ(kmers[0], "A");
  EXPECT_EQ(kmers[1], "C");
}
