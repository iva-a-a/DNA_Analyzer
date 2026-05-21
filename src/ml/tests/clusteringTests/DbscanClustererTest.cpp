#include <gtest/gtest.h>

#include <memory>
#include <set>
#include <vector>

#include "../../clustering/ClusteringContext.hpp"
#include "../../clustering/DbscanClusterer.hpp"
#include "../../clustering/IEpsProvider.hpp"
#include "../../common/Errors.hpp"

namespace {

class TestEpsProvider : public IEpsProvider {
public:
  explicit TestEpsProvider(double eps) : _eps(eps) {}

  double epsForKmerLength(int kmerLength) const override {
    (void)kmerLength;
    return _eps;
  }

private:
  double _eps;
};

std::shared_ptr<IEpsProvider> makeEpsProvider(double eps) {
  return std::make_shared<TestEpsProvider>(eps);
}

ClusteringContext makeContext(int expectedClusterCount = 0,
                              int kmerLength = 1) {
  ClusteringContext context;
  context.expectedClusterCount = expectedClusterCount;
  context.kmerLength = kmerLength;
  return context;
}

} // namespace

TEST(DbscanClustererTest, NameReturnsDbscan) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), 2);

  EXPECT_EQ(clusterer.name(), "DBSCAN");
}

TEST(DbscanClustererTest, FitPredictFormsClusterForDensePoints) {
  DbscanClusterer clusterer(makeEpsProvider(0.5), 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {0.1, 0.0},
      {0.2, 0.0},
  };
  const ClusteringContext context = makeContext();

  ClusteringResult result = clusterer.fitPredict(data, context);

  ASSERT_EQ(result.labels.size(), 3U);
  EXPECT_EQ(result.labels[0], result.labels[1]);
  EXPECT_EQ(result.labels[1], result.labels[2]);
  EXPECT_GE(result.labels[0], 0);
}

TEST(DbscanClustererTest, FitPredictMarksIsolatedPointAsNoise) {
  DbscanClusterer clusterer(makeEpsProvider(0.5), 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {0.1, 0.0},
      {100.0, 100.0},
  };
  const ClusteringContext context = makeContext();

  ClusteringResult result = clusterer.fitPredict(data, context);

  ASSERT_EQ(result.labels.size(), 3U);
  EXPECT_GE(result.labels[0], 0);
  EXPECT_GE(result.labels[1], 0);
  EXPECT_EQ(result.labels[2], -1);
}

TEST(DbscanClustererTest, FitPredictFindsTwoClusters) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {0.0, 0.1},
      {10.0, 10.0},
      {10.0, 10.1},
  };
  const ClusteringContext context = makeContext();

  ClusteringResult result = clusterer.fitPredict(data, context);

  EXPECT_NE(result.labels[0], result.labels[2]);
  EXPECT_EQ(result.labels[0], result.labels[1]);
  EXPECT_EQ(result.labels[2], result.labels[3]);
}

TEST(DbscanClustererTest, IgnoresExpectedClusterCountParameter) {
  DbscanClusterer clusterer(makeEpsProvider(0.5), 2);
  std::vector<std::vector<double>> data = {{0.0}, {0.1}, {0.2}};

  const ClusteringContext contextWithZero = makeContext(0);
  const ClusteringContext contextWithLarge = makeContext(100);

  ClusteringResult withZero = clusterer.fitPredict(data, contextWithZero);
  ClusteringResult withLarge = clusterer.fitPredict(data, contextWithLarge);

  EXPECT_EQ(withZero.labels, withLarge.labels);
}

TEST(DbscanClustererTest, UsesKmerLengthFromContextToGetEps) {
  class KmerBasedEpsProvider : public IEpsProvider {
  public:
    double epsForKmerLength(int kmerLength) const override {
      if (kmerLength == 1) {
        return 0.01;
      }

      return 1.0;
    }
  };

  DbscanClusterer clusterer(std::make_shared<KmerBasedEpsProvider>(), 2);

  std::vector<std::vector<double>> data = {
      {0.0},
      {0.5},
      {1.0},
  };

  const ClusteringContext contextWithSmallEps = makeContext(0, 1);
  const ClusteringContext contextWithLargeEps = makeContext(0, 2);

  ClusteringResult smallEpsResult =
      clusterer.fitPredict(data, contextWithSmallEps);
  ClusteringResult largeEpsResult =
      clusterer.fitPredict(data, contextWithLargeEps);

  for (int label : smallEpsResult.labels) {
    EXPECT_EQ(label, -1);
  }

  EXPECT_EQ(largeEpsResult.labels[0], largeEpsResult.labels[1]);
  EXPECT_EQ(largeEpsResult.labels[1], largeEpsResult.labels[2]);
}

TEST(DbscanClustererTest, ThrowsWhenDataIsEmpty) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), 2);
  std::vector<std::vector<double>> data;
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenEpsProviderIsNull) {
  DbscanClusterer clusterer(nullptr, 2);
  std::vector<std::vector<double>> data = {{1.0}};
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenEpsIsZero) {
  DbscanClusterer clusterer(makeEpsProvider(0.0), 2);
  std::vector<std::vector<double>> data = {{1.0}};
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenEpsIsNegative) {
  DbscanClusterer clusterer(makeEpsProvider(-1.0), 2);
  std::vector<std::vector<double>> data = {{1.0}};
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenMinPointsIsZero) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), 0);
  std::vector<std::vector<double>> data = {{1.0}};
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenMinPointsIsNegative) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), -1);
  std::vector<std::vector<double>> data = {{1.0}};
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenVectorsAreEmpty) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), 2);
  std::vector<std::vector<double>> data = {{}};
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, ThrowsWhenVectorDimensionsDiffer) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), 2);
  std::vector<std::vector<double>> data = {{1.0, 2.0}, {3.0}};
  const ClusteringContext context = makeContext();

  EXPECT_THROW(clusterer.fitPredict(data, context), ClusteringError);
}

TEST(DbscanClustererTest, AllPointsAreNoiseWhenEpsIsVerySmall) {
  DbscanClusterer clusterer(makeEpsProvider(0.01), 2);
  std::vector<std::vector<double>> data = {
      {0.0, 0.0},
      {1.0, 1.0},
      {2.0, 2.0},
  };
  const ClusteringContext context = makeContext();

  ClusteringResult result = clusterer.fitPredict(data, context);

  for (int label : result.labels) {
    EXPECT_EQ(label, -1);
  }
}

TEST(DbscanClustererTest, SinglePointWithMinPointsOneFormsCluster) {
  DbscanClusterer clusterer(makeEpsProvider(1.0), 1);
  std::vector<std::vector<double>> data = {{5.0, 5.0}};
  const ClusteringContext context = makeContext();

  ClusteringResult result = clusterer.fitPredict(data, context);

  ASSERT_EQ(result.labels.size(), 1U);
  EXPECT_EQ(result.labels[0], 0);
}