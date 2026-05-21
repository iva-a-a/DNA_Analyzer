#pragma once

#include "ClusteringContext.hpp"
#include "ClusteringResult.hpp"

class IClusterer {
public:
  virtual ~IClusterer() = default;
  virtual std::string name() const = 0;
  virtual ClusteringResult
  fitPredict(const std::vector<std::vector<double>> &data,
             const ClusteringContext &context) const = 0;
};