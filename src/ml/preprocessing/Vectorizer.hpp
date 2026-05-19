#pragma once

#include <string>
#include <vector>

class Vectorizer {
public:
  std::vector<std::vector<double>>
  fitTransform(const std::vector<std::string> &sequences, std::size_t k) const;
};