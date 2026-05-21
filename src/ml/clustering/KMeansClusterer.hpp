#pragma once

#include "IClusterer.hpp"

class KMeansClusterer : public IClusterer {
public:
  KMeansClusterer(int maxIterations, int randomSeed);
  std::string name() const override;
  ClusteringResult fitPredict(const std::vector<std::vector<double>> &data,
                              const ClusteringContext &context) const override;

private:
  int _maxIterations;
  int _randomSeed;

  void validateInput(const std::vector<std::vector<double>> &data,
                     int clusterCount) const;
  std::vector<std::vector<double>>
  initializeCentroids(const std::vector<std::vector<double>> &data,
                      int clusterCount) const;
  std::vector<int>
  assignLabels(const std::vector<std::vector<double>> &data,
               const std::vector<std::vector<double>> &centroids) const;
  std::vector<std::vector<double>>
  recomputeCentroids(const std::vector<std::vector<double>> &data,
                     const std::vector<int> &labels,
                     const std::vector<std::vector<double>> &centroids) const;
};