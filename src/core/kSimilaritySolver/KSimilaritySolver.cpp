#include "KSimilaritySolver.hpp"
#include <algorithm>
#include <climits>

int KSimilaritySolver::findMinimumSwaps(const std::string &first,
                                        const std::string &second) const {
  if (!isAnagram(first, second)) {
    return -1;
  }

  std::unordered_map<std::string, int> memo;
  std::string current = first;

  return solve(current, second, memo);
}

bool KSimilaritySolver::isAnagram(const std::string &first,
                                  const std::string &second) const {
  if (first.size() != second.size()) {
    return false;
  }

  std::unordered_map<char, int> charCount;

  for (char c : first) {
    charCount[c]++;
  }

  for (char c : second) {
    charCount[c]--;
  }

  for (const auto &pair : charCount) {
    if (pair.second != 0) {
      return false;
    }
  }

  return true;
}

int KSimilaritySolver::solve(std::string &current, const std::string &target,
                             std::unordered_map<std::string, int> &memo) const {
  if (current == target) {
    return 0;
  }

  if (memo.find(current) != memo.end()) {
    return memo[current];
  }

  size_t i = 0;

  while (i < current.size() && current[i] == target[i]) {
    i++;
  }

  int best = INT_MAX;

  for (size_t j = i + 1; j < current.size(); ++j) {
    if (current[j] == target[i] && current[j] != target[j]) {
      std::swap(current[i], current[j]);

      best = std::min(best, 1 + solve(current, target, memo));

      std::swap(current[i], current[j]);
    }
  }

  memo[current] = best;
  return best;
}