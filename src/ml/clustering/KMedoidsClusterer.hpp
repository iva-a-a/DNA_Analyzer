#pragma once

#include "IClusterer.hpp"

class KMedoidsClusterer : public IClusterer {
public:
  KMedoidsClusterer(int maxIterations, int randomSeed);
  std::string name() const override;
  ClusteringResult fitPredict(const std::vector<std::vector<double>> &data,
                              const ClusteringContext &context) const override;

private:
  int _maxIterations;
  int _randomSeed;

  void validateInput(const std::vector<std::vector<double>> &data,
                     int clusterCount) const;
  std::vector<std::size_t>
  initializeMedoids(const std::vector<std::vector<double>> &data,
                    int clusterCount) const;
  std::vector<int>
  assignLabels(const std::vector<std::vector<double>> &data,
               const std::vector<std::size_t> &medoidIndices) const;
  double totalCost(const std::vector<std::vector<double>> &data,
                   const std::vector<int> &labels,
                   const std::vector<std::size_t> &medoidIndices) const;
  bool trySwapMedoid(const std::vector<std::vector<double>> &data,
                     std::vector<std::size_t> &medoidIndices,
                     std::vector<int> &labels, int clusterId) const;
};
