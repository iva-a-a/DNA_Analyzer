#include "MinWindowService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"
#include "../../core/minWindow/MinWindowFinder.hpp"
#include "../../io/parser/InputParser.hpp"
#include "../../io/reader/FileReader.hpp"

std::string MinWindowService::run(const std::string &inputFilePath) const {
  std::vector<std::string> inputLines = FileReader::readLines(inputFilePath);
  MinWindowInput input = InputParser::parseMinWindowInput(inputLines);

  auto dnaSource = DnaSequence(input.source);
  auto dnaTarget = DnaSequence(input.target);
  auto finder = MinWindowFinder();

  return finder.findMinimumWindow(dnaSource.value(), dnaTarget.value());
}