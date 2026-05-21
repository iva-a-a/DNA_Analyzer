#include "RandIndex.hpp"

#include "../common/Errors.hpp"

double RandIndex::compute(const std::vector<int> &trueLabels,
                          const std::vector<int> &predictedLabels) const {
  if (trueLabels.size() != predictedLabels.size()) {
    throw MetricsError(
        "True labels and predicted labels must have the same size");
  }

  if (trueLabels.empty()) {
    throw MetricsError("Labels must not be empty");
  }

  int agreeingPairs = 0;
  int totalPairs = 0;

  for (std::size_t i = 0; i < trueLabels.size(); ++i) {
    for (std::size_t j = i + 1; j < trueLabels.size(); ++j) {
      bool sameTrue = (trueLabels[i] == trueLabels[j]);
      bool samePredicted = (predictedLabels[i] == predictedLabels[j]);

      if (sameTrue == samePredicted) {
        agreeingPairs++;
      }
      totalPairs++;
    }
  }

  return totalPairs > 0 ? static_cast<double>(agreeingPairs) / totalPairs : 1.0;
}