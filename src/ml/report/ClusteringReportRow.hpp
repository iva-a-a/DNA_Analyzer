#pragma once

#include <vector>

#include "ClusteringScore.hpp"

struct ClusteringReportRow {
  int kmerLength;
  std::vector<ClusteringScore> scores;
};