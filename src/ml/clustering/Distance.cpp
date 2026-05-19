#include "Distance.hpp"

#include "../common/Errors.hpp"

namespace Distance {

double euclidean(const std::vector<double> &a, const std::vector<double> &b) {
  if (a.size() != b.size()) {
    throw ClusteringError(
        "Vectors must be of the same size for distance calculation");
  }

  double sum = 0.0;

  for (size_t i = 0; i < a.size(); ++i) {
    sum += (a[i] - b[i]) * (a[i] - b[i]);
  }

  return std::sqrt(sum);
}

} // namespace Distance