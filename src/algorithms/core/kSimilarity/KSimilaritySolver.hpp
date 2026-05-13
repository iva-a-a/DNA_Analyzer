#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class KSimilaritySolver {
public:
  int findMinimumSwaps(const std::string &first,
                       const std::string &second) const;

private:
  bool isAnagram(const std::string &first, const std::string &second) const;
  std::vector<int> findMismatchPositions(const std::string &first,
                                         const std::string &second) const;
  int solve(std::string &current, const std::string &target,
            std::unordered_map<std::string, int> &memo) const;
};