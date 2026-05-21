#include <gtest/gtest.h>

#include "../../clustering/FixedTableEpsProvider.hpp"

TEST(FixedTableEpsProviderTest, ReturnsEpsForKmerLengthOne) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(1), 0.028);
}

TEST(FixedTableEpsProviderTest, ReturnsEpsForKmerLengthTwo) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(2), 0.064);
}

TEST(FixedTableEpsProviderTest, ReturnsEpsForKmerLengthThree) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(3), 0.090);
}

TEST(FixedTableEpsProviderTest, ReturnsEpsForKmerLengthFour) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(4), 0.101);
}

TEST(FixedTableEpsProviderTest, ReturnsEpsForKmerLengthFive) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(5), 0.105);
}

TEST(FixedTableEpsProviderTest, ReturnsEpsForKmerLengthSix) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(6), 0.107);
}

TEST(FixedTableEpsProviderTest, ReturnsDefaultEpsForUnknownPositiveKmerLength) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(7), 0.090);
}

TEST(FixedTableEpsProviderTest, ReturnsDefaultEpsForZeroKmerLength) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(0), 0.090);
}

TEST(FixedTableEpsProviderTest, ReturnsDefaultEpsForNegativeKmerLength) {
  FixedTableEpsProvider provider;

  EXPECT_DOUBLE_EQ(provider.epsForKmerLength(-1), 0.090);
}