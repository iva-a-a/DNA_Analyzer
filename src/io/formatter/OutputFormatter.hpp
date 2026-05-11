#pragma once

#include <string>

#include "../../core/needlemanWunsch/models/AlignmentResult.hpp"
#include "../../core/rabinKarp/model/ExactSearchResult.hpp"

namespace OutputFormatter {
std::string formatPositions(const ExactSearchResult &result);
std::string formatInt(const int value);
std::string formatAlignment(const AlignmentResult &result);
std::string formatBoolean(const bool value);
} // namespace OutputFormatter