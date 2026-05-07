#pragma once

#include <string>
#include <unordered_map>

class MinWindowFinder {
public:
  std::string findMinimumWindow(const std::string &text,
                                const std::string &pattern) const;

private:
  std::unordered_map<char, int>
  createFrequencyMap(const std::string &text) const;
};