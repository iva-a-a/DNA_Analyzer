#include "KMeansClusterer.hpp"

#include <algorithm>
#include <numeric>
#include <random>

#include "../common/Errors.hpp"
#include "Distance.hpp"

KMeansClusterer::KMeansClusterer(int maxIterations, int randomSeed)
    : _maxIterations(maxIterations), _randomSeed(randomSeed) {}

std::string KMeansClusterer::name() const { return "KMeans"; }

ClusteringResult
KMeansClusterer::fitPredict(const std::vector<std::vector<double>> &data,
                            const ClusteringContext &context) const {
  int expectedClusterCount = context.expectedClusterCount;
  validateInput(data, expectedClusterCount);

  std::vector<std::vector<double>> centroids =
      initializeCentroids(data, expectedClusterCount);
  std::vector<int> labels(data.size(), -1);

  for (int iteration = 0; iteration < _maxIterations; ++iteration) {
    std::vector<int> newLabels = assignLabels(data, centroids);

    if (newLabels == labels) {
      break;
    }

    labels = std::move(newLabels);
    centroids = recomputeCentroids(data, labels, centroids);
  }
  return ClusteringResult{labels};
}

void KMeansClusterer::validateInput(
    const std::vector<std::vector<double>> &data, int clusterCount) const {
  if (data.empty()) {
    throw ClusteringError("KMeans input data is empty");
  }

  if (clusterCount <= 0) {
    throw ClusteringError("KMeans cluster count must be positive");
  }

  if (static_cast<std::size_t>(clusterCount) > data.size()) {
    throw ClusteringError(
        "KMeans cluster count cannot be greater than data size");
  }

  if (_maxIterations <= 0) {
    throw ClusteringError("KMeans max iterations must be positive");
  }

  if (data[0].empty()) {
    throw ClusteringError("KMeans vectors must not be empty");
  }

  const std::size_t dimension = data[0].size();

  for (std::size_t i = 1; i < data.size(); ++i) {
    if (data[i].size() != dimension) {
      throw ClusteringError("KMeans vectors must have the same dimension");
    }
  }
}

std::vector<std::vector<double>> KMeansClusterer::initializeCentroids(
    const std::vector<std::vector<double>> &data, int clusterCount) const {
  std::vector<std::size_t> indices(data.size());
  std::iota(indices.begin(), indices.end(), 0);

  std::shuffle(indices.begin(), indices.end(),
               std::default_random_engine(_randomSeed));

  std::vector<std::vector<double>> centroids;
  centroids.reserve(clusterCount);

  for (int i = 0; i < clusterCount; ++i) {
    centroids.push_back(data[indices[i]]);
  }

  return centroids;
}

std::vector<int> KMeansClusterer::assignLabels(
    const std::vector<std::vector<double>> &data,
    const std::vector<std::vector<double>> &centroids) const {
  std::vector<int> labels;
  labels.reserve(data.size());
  for (const auto &point : data) {
    double minDistance = Distance::euclidean(point, centroids[0]);
    int closestCentroid = 0;
    for (std::size_t j = 1; j < centroids.size(); ++j) {
      double distance = Distance::euclidean(point, centroids[j]);
      if (distance < minDistance) {
        minDistance = distance;
        closestCentroid = static_cast<int>(j);
      }
    }
    labels.push_back(closestCentroid);
  }
  return labels;
}

std::vector<std::vector<double>> KMeansClusterer::recomputeCentroids(
    const std::vector<std::vector<double>> &data,
    const std::vector<int> &labels,
    const std::vector<std::vector<double>> &centroids) const {
  std::vector<std::vector<double>> newCentroids(
      centroids.size(), std::vector<double>(centroids[0].size(), 0.0));
  std::vector<std::size_t> clusterCounts(centroids.size(), 0);

  for (std::size_t i = 0; i < data.size(); ++i) {
    int label = labels[i];
    clusterCounts[label]++;
    for (std::size_t j = 0; j < data[i].size(); ++j) {
      newCentroids[label][j] += data[i][j];
    }
  }

  for (std::size_t i = 0; i < newCentroids.size(); ++i) {
    if (clusterCounts[i] == 0) {
      newCentroids[i] = centroids[i];
    } else {
      for (std::size_t j = 0; j < newCentroids[i].size(); ++j) {
        newCentroids[i][j] /= static_cast<double>(clusterCounts[i]);
      }
    }
  }
  return newCentroids;
}