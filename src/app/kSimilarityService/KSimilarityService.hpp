#pragma once

#include "../../core/kSimilarity/KSimilaritySolver.hpp"

class KSimilarityService {
public:
  int search(const std::string &first, const std::string &second) const;

private:
  KSimilaritySolver _solver;
};