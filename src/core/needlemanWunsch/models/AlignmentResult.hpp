#pragma once

#include <string>

struct AlignmentResult {
  int score;
  std::string alignedFirst;
  std::string matchLine;
  std::string alignedSecond;
};