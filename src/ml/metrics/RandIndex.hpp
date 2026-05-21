#pragma once

#include <vector>

class RandIndex {
public:
  double compute(const std::vector<int> &trueLabels,
                 const std::vector<int> &predictedLabels) const;
};