#include "InputParser.hpp"

#include <sstream>

#include "../../core/common/errors/Errors.hpp"

namespace InputParser {

AlignmentInput parseAlignmentInput(const std::vector<std::string> &lines) {
  if (lines.size() != 3) {
    throw InputFormatError("Alignment input must consist of exactly 3 lines");
  }

  ScoringScheme scoring;
  std::istringstream iss(lines[0]);

  if (!(iss >> scoring.matchScore >> scoring.mismatchScore >>
        scoring.gapScore)) {
    throw InputFormatError(
        "First line must contain 3 integers: match mismatch gap");
  }

  return {scoring, lines[1], lines[2]};
}

ExactSearchInput
parseExactSearchInput(const std::vector<std::string> &textLines,
                      const std::vector<std::string> &patternLines) {
  if (textLines.size() != 1) {
    throw InputFormatError("Text file must contain exactly one line");
  }

  if (patternLines.size() != 1) {
    throw InputFormatError("Pattern file must contain exactly one line");
  }

  const std::string &text = textLines[0];
  const std::string &pattern = patternLines[0];

  if (text.empty() || text.size() > 10000) {
    throw InputFormatError("Text cannot be empty or exceed 10000 characters");
  }

  if (pattern.empty() || pattern.size() > 100) {
    throw InputFormatError("Pattern cannot be empty or exceed 100 characters");
  }

  if (pattern.size() > text.size()) {
    throw InputFormatError("Pattern length cannot exceed text length");
  }

  return {text, pattern};
}

KSimilarityInput parseKSimilarityInput(const std::vector<std::string> &lines) {
  if (lines.size() != 2) {
    throw InputFormatError(
        "K-similarity input must consist of exactly 2 lines");
  }

  return {lines[0], lines[1]};
}

MinWindowInput parseMinWindowInput(const std::vector<std::string> &lines) {
  if (lines.size() != 2) {
    throw InputFormatError("Min window input must consist of exactly 2 lines");
  }

  return {lines[0], lines[1]};
}

RegexMatchInput parseRegexMatchInput(const std::vector<std::string> &lines) {
  if (lines.size() != 2) {
    throw InputFormatError("Regex match input must consist of exactly 2 lines");
  }

  return {lines[0], lines[1]};
}

} // namespace InputParser