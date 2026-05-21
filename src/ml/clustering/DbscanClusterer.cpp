#include "DbscanClusterer.hpp"

#include <queue>

#include "../common/Errors.hpp"
#include "Distance.hpp"

DbscanClusterer::DbscanClusterer(std::shared_ptr<IEpsProvider> epsProvider,
                                 int minPoints)
    : _epsProvider(std::move(epsProvider)), _minPoints(minPoints) {}

std::string DbscanClusterer::name() const { return "DBSCAN"; }

ClusteringResult
DbscanClusterer::fitPredict(const std::vector<std::vector<double>> &data,
                            const ClusteringContext &context) const {
  if (!_epsProvider) {
    throw ClusteringError("DBSCAN eps provider is not set");
  }

  const double eps = _epsProvider->epsForKmerLength(context.kmerLength);
  validateInput(data, eps);

  ClusteringResult result;
  result.labels.resize(data.size(), -1);
  std::vector<bool> visited(data.size(), false);
  int clusterId = 0;

  for (std::size_t i = 0; i < data.size(); ++i) {
    if (visited[i]) {
      continue;
    }

    visited[i] = true;
    std::vector<int> neighbors = regionQuery(data, i, eps);

    if (neighbors.size() < static_cast<std::size_t>(_minPoints)) {
      result.labels[i] = -1;
    } else {
      expandCluster(data, i, neighbors, clusterId, result.labels, visited, eps);
      clusterId++;
    }
  }

  return result;
}

void DbscanClusterer::validateInput(
    const std::vector<std::vector<double>> &data, double eps) const {
  if (data.empty()) {
    throw ClusteringError("DBSCAN input data is empty");
  }

  if (eps <= 0.0) {
    throw ClusteringError("DBSCAN eps must be positive");
  }

  if (_minPoints <= 0) {
    throw ClusteringError("DBSCAN minPoints must be positive");
  }

  if (data[0].empty()) {
    throw ClusteringError("DBSCAN vectors must not be empty");
  }

  const std::size_t dimension = data[0].size();

  for (std::size_t i = 1; i < data.size(); ++i) {
    if (data[i].size() != dimension) {
      throw ClusteringError("DBSCAN vectors must have the same dimension");
    }
  }
}

std::vector<int>
DbscanClusterer::regionQuery(const std::vector<std::vector<double>> &data,
                             std::size_t pointIndex, double eps) const {
  std::vector<int> neighbors;

  for (std::size_t i = 0; i < data.size(); ++i) {
    if (Distance::euclidean(data[pointIndex], data[i]) <= eps) {
      neighbors.push_back(static_cast<int>(i));
    }
  }

  return neighbors;
}

void DbscanClusterer::expandCluster(
    const std::vector<std::vector<double>> &data, std::size_t pointIndex,
    const std::vector<int> &neighbors, int clusterId, std::vector<int> &labels,
    std::vector<bool> &visited, double eps) const {
  labels[pointIndex] = clusterId;
  std::queue<int> neighborsQueue;

  for (int neighbor : neighbors) {
    neighborsQueue.push(neighbor);
  }

  while (!neighborsQueue.empty()) {
    int currentIndex = neighborsQueue.front();
    neighborsQueue.pop();

    if (!visited[currentIndex]) {
      visited[currentIndex] = true;
      std::vector<int> currentNeighbors = regionQuery(data, currentIndex, eps);

      if (currentNeighbors.size() >= static_cast<std::size_t>(_minPoints)) {
        for (int neighbor : currentNeighbors) {
          neighborsQueue.push(neighbor);
        }
      }
    }

    if (labels[currentIndex] == -1) {
      labels[currentIndex] = clusterId;
    }
  }
}