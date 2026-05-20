#include <gtest/gtest.h>

#include <cmath>

#include "../../common/Errors.hpp"
#include "../../preprocessing/Vectorizer.hpp"

TEST(VectorizerTest, FitTransformProducesNormalizedVectors) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences = {"ACGT", "ACGT"};

  std::vector<std::vector<double>> vectors =
      vectorizer.fitTransform(sequences, 1);

  ASSERT_EQ(vectors.size(), 2U);
  ASSERT_EQ(vectors[0].size(), 4U);
  ASSERT_EQ(vectors[1].size(), 4U);

  for (double value : vectors[0]) {
    EXPECT_DOUBLE_EQ(value, 0.25);
  }
  EXPECT_EQ(vectors[0], vectors[1]);
}

TEST(VectorizerTest, FitTransformBuildsSharedVocabularyAcrossSequences) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences = {"AC", "CG"};

  std::vector<std::vector<double>> vectors =
      vectorizer.fitTransform(sequences, 1);

  ASSERT_EQ(vectors.size(), 2U);
  ASSERT_EQ(vectors[0].size(), 3U);
  EXPECT_DOUBLE_EQ(vectors[0][0], 0.5);
  EXPECT_DOUBLE_EQ(vectors[0][1], 0.5);
  EXPECT_DOUBLE_EQ(vectors[0][2], 0.0);
  EXPECT_DOUBLE_EQ(vectors[1][0], 0.0);
  EXPECT_DOUBLE_EQ(vectors[1][1], 0.5);
  EXPECT_DOUBLE_EQ(vectors[1][2], 0.5);
}

TEST(VectorizerTest, FitTransformWithKmerLengthGreaterThanOne) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences = {"ACGT"};

  std::vector<std::vector<double>> vectors =
      vectorizer.fitTransform(sequences, 2);

  ASSERT_EQ(vectors.size(), 1U);
  ASSERT_EQ(vectors[0].size(), 3U);

  double sum = 0.0;
  for (double value : vectors[0]) {
    sum += value;
  }
  EXPECT_NEAR(sum, 1.0, 1e-9);
}

TEST(VectorizerTest, ThrowsWhenSequenceListIsEmpty) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences;

  EXPECT_THROW(vectorizer.fitTransform(sequences, 1), ValidationError);
}

TEST(VectorizerTest, ThrowsWhenNoKmersCanBeExtracted) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences = {"AC", "GT"};

  EXPECT_THROW(vectorizer.fitTransform(sequences, 3), ValidationError);
}

TEST(VectorizerTest, ThrowsWhenAllSequencesAreTooShortForK) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences = {"", "A"};

  EXPECT_THROW(vectorizer.fitTransform(sequences, 2), ValidationError);
}

TEST(VectorizerTest, HandlesSequenceShorterThanKBySkippingItsKmers) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences = {"ACGT", "A"};

  std::vector<std::vector<double>> vectors =
      vectorizer.fitTransform(sequences, 2);

  ASSERT_EQ(vectors.size(), 2U);
  EXPECT_GT(vectors[0].size(), 0U);
  EXPECT_DOUBLE_EQ(vectors[1][0], 0.0);
}

TEST(VectorizerTest, SingleSequenceProducesNonZeroVector) {
  Vectorizer vectorizer;
  std::vector<std::string> sequences = {"AAA"};

  std::vector<std::vector<double>> vectors =
      vectorizer.fitTransform(sequences, 1);

  ASSERT_EQ(vectors.size(), 1U);
  EXPECT_DOUBLE_EQ(vectors[0][0], 1.0);
}
