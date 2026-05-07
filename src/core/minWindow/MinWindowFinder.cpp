#include "MinWindowFinder.hpp"

std::string
MinWindowFinder::findMinimumWindow(const std::string &text,
                                   const std::string &pattern) const {
  if (pattern.empty() || pattern.size() > text.size()) {
    return "";
  }
  std::unordered_map<char, int> need = createFrequencyMap(pattern);

  size_t left = 0;
  size_t best_start = 0;
  size_t best_length = std::numeric_limits<size_t>::max();

  std::unordered_map<char, int> window;

  size_t formed = 0;
  size_t required = need.size();

  for (size_t right = 0; right < text.size(); ++right) {
    char currentChar = text[right];
    window[currentChar]++;
    if (need.contains(currentChar) &&
        window[currentChar] == need[currentChar]) {
      formed++;
    }
    while (left <= right && formed == required) {
      size_t current_length = right - left + 1;

      if (current_length < best_length) {
        best_start = left;
        best_length = current_length;
      }
      char leftChar = text[left];
      window[leftChar]--;
      if (need.count(leftChar) > 0 && window[leftChar] < need[leftChar]) {
        formed--;
      }
      left++;
    }
  }
  return best_length == std::numeric_limits<size_t>::max()
             ? ""
             : text.substr(best_start, best_length);
}

std::unordered_map<char, int>
MinWindowFinder::createFrequencyMap(const std::string &text) const {
  std::unordered_map<char, int> frequencyMap;
  for (char c : text) {
    frequencyMap[c]++;
  }
  return frequencyMap;
}