#pragma once

#include <vector>

#include "dto/AlignmentInput.hpp"
#include "dto/ExactSearchInput.hpp"
#include "dto/KSimilarityInput.hpp"
#include "dto/MinWindowInput.hpp"
#include "dto/RegexMatchInput.hpp"

namespace InputParser {
AlignmentInput parseAlignmentInput(const std::vector<std::string> &lines);
ExactSearchInput
parseExactSearchInput(const std::vector<std::string> &textLines,
                      const std::vector<std::string> &patternLines);
KSimilarityInput parseKSimilarityInput(const std::vector<std::string> &lines);
MinWindowInput parseMinWindowInput(const std::vector<std::string> &lines);
RegexMatchInput parseRegexMatchInput(const std::vector<std::string> &lines);
} // namespace InputParser