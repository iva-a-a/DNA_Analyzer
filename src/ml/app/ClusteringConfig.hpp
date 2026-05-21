#pragma once

#include <string>

struct ClusteringConfig {
  std::string inputPath;
  std::string outputReportPath;
  int minKmerLength = 1;
  int maxKmerLength = 6;
  int randomSeed = 42;
  int kmeansMaxIterations = 100;
  int dbscanMinPoints = 3;
  int kmedoidsMaxIterations = 100;
};