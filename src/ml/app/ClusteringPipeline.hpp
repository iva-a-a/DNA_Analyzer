#pragma once

#include "ClusteringConfig.hpp"

class ClusteringPipeline {
public:
  void run(const ClusteringConfig &config) const;
};