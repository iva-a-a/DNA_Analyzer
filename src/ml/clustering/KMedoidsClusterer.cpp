#include "KMedoidsClusterer.hpp"

#include <algorithm>
#include <numeric>
#include <random>

#include "../common/Errors.hpp"
#include "Distance.hpp"

KMedoidsClusterer::KMedoidsClusterer(int maxIterations, int randomSeed)
    : _maxIterations(maxIterations), _randomSeed(randomSeed) {}

std::string KMedoidsClusterer::name() const { return "KMedoids"; }

ClusteringResult
KMedoidsClusterer::fitPredict(const std::vector<std::vector<double>> &data,
                              const ClusteringContext &context) const {
  int expectedClusterCount = context.expectedClusterCount;
  validateInput(data, expectedClusterCount);

  std::vector<std::size_t> medoidIndices =
      initializeMedoids(data, expectedClusterCount);
  std::vector<int> labels = assignLabels(data, medoidIndices);

  for (int iteration = 0; iteration < _maxIterations; ++iteration) {
    bool improved = false;

    for (int clusterId = 0; clusterId < expectedClusterCount; ++clusterId) {
      if (trySwapMedoid(data, medoidIndices, labels, clusterId)) {
        improved = true;
      }
    }

    std::vector<int> newLabels = assignLabels(data, medoidIndices);

    if (newLabels == labels && !improved) {
      break;
    }

    labels = std::move(newLabels);
  }

  return ClusteringResult{labels};
}

void KMedoidsClusterer::validateInput(
    const std::vector<std::vector<double>> &data, int clusterCount) const {
  if (data.empty()) {
    throw ClusteringError("KMedoids input data is empty");
  }

  if (clusterCount <= 0) {
    throw ClusteringError("KMedoids cluster count must be positive");
  }

  if (static_cast<std::size_t>(clusterCount) > data.size()) {
    throw ClusteringError(
        "KMedoids cluster count cannot be greater than data size");
  }

  if (_maxIterations <= 0) {
    throw ClusteringError("KMedoids max iterations must be positive");
  }

  if (data[0].empty()) {
    throw ClusteringError("KMedoids vectors must not be empty");
  }

  const std::size_t dimension = data[0].size();

  for (std::size_t i = 1; i < data.size(); ++i) {
    if (data[i].size() != dimension) {
      throw ClusteringError("KMedoids vectors must have the same dimension");
    }
  }
}

std::vector<std::size_t> KMedoidsClusterer::initializeMedoids(
    const std::vector<std::vector<double>> &data, int clusterCount) const {
  std::vector<std::size_t> indices(data.size());
  std::iota(indices.begin(), indices.end(), 0);

  std::shuffle(indices.begin(), indices.end(),
               std::default_random_engine(_randomSeed));

  std::vector<std::size_t> medoidIndices;
  medoidIndices.reserve(clusterCount);

  for (int i = 0; i < clusterCount; ++i) {
    medoidIndices.push_back(indices[i]);
  }

  return medoidIndices;
}

std::vector<int> KMedoidsClusterer::assignLabels(
    const std::vector<std::vector<double>> &data,
    const std::vector<std::size_t> &medoidIndices) const {
  std::vector<int> labels;
  labels.reserve(data.size());

  for (const auto &point : data) {
    double minDistance = Distance::euclidean(point, data[medoidIndices[0]]);
    int closestMedoid = 0;

    for (std::size_t j = 1; j < medoidIndices.size(); ++j) {
      double distance = Distance::euclidean(point, data[medoidIndices[j]]);

      if (distance < minDistance) {
        minDistance = distance;
        closestMedoid = static_cast<int>(j);
      }
    }

    labels.push_back(closestMedoid);
  }

  return labels;
}

double KMedoidsClusterer::totalCost(
    const std::vector<std::vector<double>> &data,
    const std::vector<int> &labels,
    const std::vector<std::size_t> &medoidIndices) const {
  double cost = 0.0;

  for (std::size_t i = 0; i < data.size(); ++i) {
    cost += Distance::euclidean(data[i], data[medoidIndices[labels[i]]]);
  }

  return cost;
}

bool KMedoidsClusterer::trySwapMedoid(
    const std::vector<std::vector<double>> &data,
    std::vector<std::size_t> &medoidIndices, std::vector<int> &labels,
    int clusterId) const {
  const std::size_t currentMedoidIndex = medoidIndices[clusterId];
  double bestCost = totalCost(data, labels, medoidIndices);
  std::size_t bestMedoidIndex = currentMedoidIndex;
  bool improved = false;

  for (std::size_t pointIndex = 0; pointIndex < data.size(); ++pointIndex) {
    if (labels[pointIndex] != clusterId) {
      continue;
    }

    if (pointIndex == currentMedoidIndex) {
      continue;
    }

    medoidIndices[clusterId] = pointIndex;
    const std::vector<int> candidateLabels = assignLabels(data, medoidIndices);
    const double candidateCost =
        totalCost(data, candidateLabels, medoidIndices);

    if (candidateCost < bestCost) {
      bestCost = candidateCost;
      bestMedoidIndex = pointIndex;
      improved = true;
    }
  }

  medoidIndices[clusterId] = bestMedoidIndex;

  if (improved) {
    labels = assignLabels(data, medoidIndices);
  }

  return improved;
}
