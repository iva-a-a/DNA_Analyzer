#include <gtest/gtest.h>

#include <set>

#include "../../clustering/DbscanClusterer.hpp"
#include "../../common/Errors.hpp"

TEST(DbscanClustererTest, NameReturnsDbscan) {
  DbscanClusterer clusterer(1.0, 2);

  EXPECT_EQ(clusterer.name(), "DBSCAN");
}

TEST(DbscanClustererTest, FitPredictFormsClusterForDensePoints) {
  DbscanClusterer clusterer(0.5, 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {0.1, 0.0},
      {0.2, 0.0},
  };

  ClusteringResult result = clusterer.fitPredict(data, 0);

  ASSERT_EQ(result.labels.size(), 3U);
  EXPECT_EQ(result.labels[0], result.labels[1]);
  EXPECT_EQ(result.labels[1], result.labels[2]);
  EXPECT_GE(result.labels[0], 0);
}

TEST(DbscanClustererTest, FitPredictMarksIsolatedPointAsNoise) {
  DbscanClusterer clusterer(0.5, 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {0.1, 0.0},
      {100.0, 100.0},
  };

  ClusteringResult result = clusterer.fitPredict(data, 0);

  ASSERT_EQ(result.labels.size(), 3U);
  EXPECT_GE(result.labels[0], 0);
  EXPECT_GE(result.labels[1], 0);
  EXPECT_EQ(result.labels[2], -1);
}

TEST(DbscanClustererTest, FitPredictFindsTwoClusters) {
  DbscanClusterer clusterer(1.0, 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {0.0, 0.1},
      {10.0, 10.0},
      {10.0, 10.1},
  };

  ClusteringResult result = clusterer.fitPredict(data, 0);

  EXPECT_NE(result.labels[0], result.labels[2]);
  EXPECT_EQ(result.labels[0], result.labels[1]);
  EXPECT_EQ(result.labels[2], result.labels[3]);
}

TEST(DbscanClustererTest, IgnoresExpectedClusterCountParameter) {
  DbscanClusterer clusterer(0.5, 2);
  std::vector<std::vector<double>> data = {{0.0}, {0.1}, {0.2}};

  ClusteringResult withZero =
      clusterer.fitPredict(data, 0);
  ClusteringResult withLarge =
      clusterer.fitPredict(data, 100);

  EXPECT_EQ(withZero.labels, withLarge.labels);
}

TEST(DbscanClustererTest, ThrowsWhenDataIsEmpty) {
  DbscanClusterer clusterer(1.0, 2);
  std::vector<std::vector<double>> data;

  EXPECT_THROW(clusterer.fitPredict(data, 0), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenEpsIsZero) {
  DbscanClusterer clusterer(0.0, 2);
  std::vector<std::vector<double>> data = {{1.0}};

  EXPECT_THROW(clusterer.fitPredict(data, 0), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenEpsIsNegative) {
  DbscanClusterer clusterer(-1.0, 2);
  std::vector<std::vector<double>> data = {{1.0}};

  EXPECT_THROW(clusterer.fitPredict(data, 0), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenMinPointsIsZero) {
  DbscanClusterer clusterer(1.0, 0);
  std::vector<std::vector<double>> data = {{1.0}};

  EXPECT_THROW(clusterer.fitPredict(data, 0), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenMinPointsIsNegative) {
  DbscanClusterer clusterer(1.0, -1);
  std::vector<std::vector<double>> data = {{1.0}};

  EXPECT_THROW(clusterer.fitPredict(data, 0), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenVectorsAreEmpty) {
  DbscanClusterer clusterer(1.0, 2);
  std::vector<std::vector<double>> data = {{}};

  EXPECT_THROW(clusterer.fitPredict(data, 0), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenVectorDimensionsDiffer) {
  DbscanClusterer clusterer(1.0, 2);
  std::vector<std::vector<double>> data = {{1.0, 2.0}, {3.0}};

  EXPECT_THROW(clusterer.fitPredict(data, 0), ClusteringError);
}

TEST(DbscanClustererTest, AllPointsAreNoiseWhenEpsIsVerySmall) {
  DbscanClusterer clusterer(0.01, 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {1.0, 1.0},
      {2.0, 2.0},
  };

  ClusteringResult result = clusterer.fitPredict(data, 0);

  for (int label : result.labels) {
    EXPECT_EQ(label, -1);
  }
}

TEST(DbscanClustererTest, SinglePointWithMinPointsOneFormsCluster) {
  DbscanClusterer clusterer(1.0, 1);
  std::vector<std::vector<double>> data = {{5.0, 5.0}};

  ClusteringResult result = clusterer.fitPredict(data, 0);

  ASSERT_EQ(result.labels.size(), 1U);
  EXPECT_EQ(result.labels[0], 0);
}
