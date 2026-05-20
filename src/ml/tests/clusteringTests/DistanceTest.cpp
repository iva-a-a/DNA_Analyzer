#include <gtest/gtest.h>

#include <cmath>

#include "../../clustering/Distance.hpp"
#include "../../common/Errors.hpp"

TEST(DistanceTest, EuclideanDistanceOfIdenticalVectorsIsZero) {
  std::vector<double> vector = {1.0, 2.0, 3.0};

  EXPECT_DOUBLE_EQ(Distance::euclidean(vector, vector), 0.0);
}

TEST(DistanceTest, EuclideanDistanceOfEmptyVectorsIsZero) {
  std::vector<double> empty;

  EXPECT_DOUBLE_EQ(Distance::euclidean(empty, empty), 0.0);
}

TEST(DistanceTest, EuclideanDistanceComputesCorrectValue) {
  std::vector<double> a = {0.0, 0.0};
  std::vector<double> b = {3.0, 4.0};

  EXPECT_DOUBLE_EQ(Distance::euclidean(a, b), 5.0);
}

TEST(DistanceTest, EuclideanDistanceIsSymmetric) {
  std::vector<double> a = {1.0, 2.0, 3.0};
  std::vector<double> b = {4.0, 5.0, 6.0};

  EXPECT_DOUBLE_EQ(Distance::euclidean(a, b), Distance::euclidean(b, a));
}

TEST(DistanceTest, ThrowsClusteringErrorWhenVectorSizesDiffer) {
  std::vector<double> a = {1.0, 2.0};
  std::vector<double> b = {1.0};

  EXPECT_THROW(Distance::euclidean(a, b), ClusteringError);
  EXPECT_THROW(Distance::euclidean(b, a), ClusteringError);
}

TEST(DistanceTest, ThrowsClusteringErrorWithExpectedMessage) {
  std::vector<double> a = {1.0};
  std::vector<double> b = {1.0, 2.0};

  try {
    Distance::euclidean(a, b);
    FAIL() << "Expected ClusteringError";
  } catch (const ClusteringError &error) {
    EXPECT_STREQ(error.what(),
                 "Vectors must be of the same size for distance calculation");
  }
}
