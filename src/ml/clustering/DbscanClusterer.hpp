#pragma once

#include "IClusterer.hpp"

class DbscanClusterer : public IClusterer {
public:
  DbscanClusterer(double eps, int minPoints);
  std::string name() const override;
  ClusteringResult fitPredict(const std::vector<std::vector<double>> &data,
                              int expectedClusterCount) const override;

private:
  double _eps;
  int _minPoints;

  void validateInput(const std::vector<std::vector<double>> &data) const;
  std::vector<int> regionQuery(const std::vector<std::vector<double>> &data,
                               std::size_t pointIndex) const;
  void expandCluster(const std::vector<std::vector<double>> &data,
                     std::size_t pointIndex, const std::vector<int> &neighbors,
                     int clusterId, std::vector<int> &labels,
                     std::vector<bool> &visited) const;
};