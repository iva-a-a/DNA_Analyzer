#pragma once

#include "../../core/needlemanWunsch/models/AlignmentResult.hpp"

class AlignmentService {
public:
  int runScoreOnly(const std::string &inputFilePath) const;
  AlignmentResult runFullAlignment(const std::string &inputFilePath) const;
};