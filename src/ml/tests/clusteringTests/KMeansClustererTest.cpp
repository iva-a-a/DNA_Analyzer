#include <gtest/gtest.h>

#include <set>
#include <vector>

#include "../../clustering/ClusteringContext.hpp"
#include "../../clustering/KMeansClusterer.hpp"
#include "../../common/Errors.hpp"

namespace {

std::vector<std::vector<double>> makeSeparatedClusters() {
  return {
      {0.0, 0.0},
      {0.0, 1.0},
      {10.0, 10.0},
      {10.0, 11.0},
  };
}

ClusteringContext makeContext(int expectedClusterCount, int kmerLength = 1) {
  ClusteringContext context;
  context.expectedClusterCount = expectedClusterCount;
  context.kmerLength = kmerLength;
  return context;
}

bool sameClusterGroup(const std::vector<int> &labels, int first, int second) {
  return labels[first] == labels[second];
}

} // namespace

TEST(KMeansClustererTest, NameReturnsKMeans) {
  KMeansClusterer clusterer(10, 42);

  EXPECT_EQ(clusterer.name(), "KMeans");
}

TEST(KMeansClustererTest, FitPredictAssignsSeparatedPointsToTwoClusters) {
  KMeansClusterer clusterer(100, 42);
  const auto data = makeSeparatedClusters();
  const ClusteringContext context = makeContext(2);

  ClusteringResult result = clusterer.fitPredict(data, context);

  ASSERT_EQ(result.labels.size(), data.size());
  EXPECT_TRUE(sameClusterGroup(result.labels, 0, 1));
  EXPECT_TRUE(sameClusterGroup(result.labels, 2, 3));
  EXPECT_FALSE(sameClusterGroup(result.labels, 0, 2));
}

TEST(KMeansClustererTest, FitPredictIsDeterministicWithFixedSeed) {
  KMeansClusterer clustererA(100, 7);
  KMeansClusterer clustererB(100, 7);
  const auto data = makeSeparatedClusters();
  const ClusteringContext context = makeContext(2);

  ClusteringResult resultA = clustererA.fitPredict(data, context);
  ClusteringResult resultB = clustererB.fitPredict(data, context);

  EXPECT_EQ(resultA.labels, resultB.labels);
}

TEST(KMeansClustererTest, FitPredictWithSinglePointAndOneCluster) {
  KMeansClusterer clusterer(10, 1);
  std::vector<std::vector<double>> data = {{5.0, 5.0}};
  const ClusteringContext context = makeContext(1);

  ClusteringResult result = clusterer.fitPredict(data, context);

  ASSERT_EQ(result.labels.size(), 1U);
  EXPECT_EQ(result.labels[0], 0);
}

TEST(KMeansClustererTest, ThrowsWhenDataIsEmpty) {
  KMeansClusterer clusterer(10, 42);
  std::vector<std::vector<double>> data;
  const ClusteringContext context = makeContext(1);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, ThrowsWhenClusterCountIsZero) {
  KMeansClusterer clusterer(10, 42);
  std::vector<std::vector<double>> data = {{1.0, 2.0}};
  const ClusteringContext context = makeContext(0);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, ThrowsWhenClusterCountIsNegative) {
  KMeansClusterer clusterer(10, 42);
  std::vector<std::vector<double>> data = {{1.0, 2.0}};
  const ClusteringContext context = makeContext(-1);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, ThrowsWhenClusterCountExceedsDataSize) {
  KMeansClusterer clusterer(10, 42);
  std::vector<std::vector<double>> data = {{1.0, 2.0}, {3.0, 4.0}};
  const ClusteringContext context = makeContext(3);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, ThrowsWhenMaxIterationsIsZero) {
  KMeansClusterer clusterer(0, 42);
  std::vector<std::vector<double>> data = {{1.0, 2.0}};
  const ClusteringContext context = makeContext(1);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, ThrowsWhenMaxIterationsIsNegative) {
  KMeansClusterer clusterer(-5, 42);
  std::vector<std::vector<double>> data = {{1.0, 2.0}};
  const ClusteringContext context = makeContext(1);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, ThrowsWhenVectorsAreEmpty) {
  KMeansClusterer clusterer(10, 42);
  std::vector<std::vector<double>> data = {{}};
  const ClusteringContext context = makeContext(1);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, ThrowsWhenVectorDimensionsDiffer) {
  KMeansClusterer clusterer(10, 42);
  std::vector<std::vector<double>> data = {{1.0, 2.0}, {3.0}};
  const ClusteringContext context = makeContext(2);

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(KMeansClustererTest, LabelsAreWithinClusterRange) {
  KMeansClusterer clusterer(50, 13);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {5.0, 5.0}, {6.0, 5.0},
  };
  const ClusteringContext context = makeContext(2);

  ClusteringResult result = clusterer.fitPredict(data, context);

  for (int label : result.labels) {
    EXPECT_GE(label, 0);
    EXPECT_LT(label, context.expectedClusterCount);
  }
}

TEST(KMeansClustererTest, UsesAllClusterIdsForWellSeparatedData) {
  KMeansClusterer clusterer(100, 42);
  const auto data = makeSeparatedClusters();
  const ClusteringContext context = makeContext(2);

  ClusteringResult result = clusterer.fitPredict(data, context);

  std::set<int> uniqueLabels(result.labels.begin(), result.labels.end());
  EXPECT_EQ(uniqueLabels.size(), 2U);
}