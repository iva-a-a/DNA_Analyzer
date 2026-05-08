#include "KSimilarityService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"
#include "../../core/kSimilarity/KSimilaritySolver.hpp"
#include "../../io/parser/InputParser.hpp"
#include "../../io/reader/FileReader.hpp"

int KSimilarityService::run(const std::string &inputFilePath) const {
  auto inputLines = FileReader::readLines(inputFilePath);
  auto input = InputParser::parseKSimilarityInput(inputLines);
  auto dnaSource = DnaSequence(input.source);
  auto dnaTarget = DnaSequence(input.target);
  auto solver = KSimilaritySolver();
  return solver.findMinimumSwaps(dnaSource.value(), dnaTarget.value());
}