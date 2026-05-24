#pragma once

#include <string>

struct ClusteringReportAnalysis {
  int bestKmerLength = 0;
  std::string bestAlgorithm;
  double bestRandIndex = 0.0;
};