#include <gtest/gtest.h>

#include "../../common/Errors.hpp"
#include "../../metrics/RandIndex.hpp"

TEST(RandIndexTest, PerfectAgreementReturnsOne) {
  RandIndex randIndex;

  EXPECT_DOUBLE_EQ(randIndex.compute({0, 0, 1, 1}, {0, 0, 1, 1}), 1.0);
}

TEST(RandIndexTest, CompleteDisagreementReturnsZero) {
  RandIndex randIndex;

  EXPECT_DOUBLE_EQ(randIndex.compute({0, 0, 0}, {0, 1, 2}), 0.0);
}

TEST(RandIndexTest, PartialAgreementReturnsExpectedValue) {
  RandIndex randIndex;

  EXPECT_NEAR(randIndex.compute({0, 0, 1, 1}, {0, 0, 0, 1}), 0.5, 1e-9);
}

TEST(RandIndexTest, ThrowsWhenLabelSizesDiffer) {
  RandIndex randIndex;

  EXPECT_THROW(randIndex.compute({0, 1}, {0}), MetricsError);
}

TEST(RandIndexTest, ThrowsWhenLabelsAreEmpty) {
  RandIndex randIndex;

  EXPECT_THROW(randIndex.compute({}, {}), MetricsError);
}
