#pragma once

#include "IClusterer.hpp"
#include "IEpsProvider.hpp"

class DbscanClusterer : public IClusterer {
public:
  DbscanClusterer(std::shared_ptr<IEpsProvider> epsProvider, int minPoints);
  std::string name() const override;
  ClusteringResult fitPredict(const std::vector<std::vector<double>> &data,
                              const ClusteringContext &context) const override;

private:
  std::shared_ptr<IEpsProvider> _epsProvider;
  int _minPoints;

  void validateInput(const std::vector<std::vector<double>> &data,
                     double eps) const;
  std::vector<int> regionQuery(const std::vector<std::vector<double>> &data,
                               std::size_t pointIndex, double eps) const;
  void expandCluster(const std::vector<std::vector<double>> &data,
                     std::size_t pointIndex, const std::vector<int> &neighbors,
                     int clusterId, std::vector<int> &labels,
                     std::vector<bool> &visited, double eps) const;
};