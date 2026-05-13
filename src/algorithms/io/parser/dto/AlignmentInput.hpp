#pragma once

#include <string>

#include "../../../core/needlemanWunsch/models/ScoringScheme.hpp"

struct AlignmentInput {
  ScoringScheme scoring;
  std::string first;
  std::string second;
};