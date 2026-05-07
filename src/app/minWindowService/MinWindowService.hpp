#pragma once

#include "../../core/minWindow/MinWindowFinder.hpp"

class MinWindowService {
public:
  std::string find(const std::string &text, const std::string &pattern) const;

private:
  MinWindowFinder _finder;
};