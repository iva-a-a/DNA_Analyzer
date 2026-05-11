#include "OutputFormatter.hpp"

namespace OutputFormatter {

std::string formatPositions(const ExactSearchResult &result) {
  const std::vector<std::size_t> &positions = result.positions;

  std::string output;

  for (std::size_t i = 0; i < positions.size(); ++i) {
    output += std::to_string(positions[i]);

    if (i + 1 < positions.size()) {
      output += " ";
    }
  }

  return output;
}

std::string formatInt(const int value) { return std::to_string(value); }

std::string formatAlignment(const AlignmentResult &result) {
  std::string output;

  output += formatInt(result.score) + "\n";
  output += result.alignedFirst + "\n";
  output += result.matchLine + "\n";
  output += result.alignedSecond;

  return output;
}

std::string formatBoolean(const bool value) { return value ? "True" : "False"; }

} // namespace OutputFormatter