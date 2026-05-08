#include "AlignmentService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"
#include "../../core/needlemanWunsch/NeedlemanWunschAligner.hpp"
#include "../../io/parser/InputParser.hpp"
#include "../../io/reader/FileReader.hpp"

int AlignmentService::runScoreOnly(const std::string &inputFilePath) const {
  auto inputLines = FileReader::readLines(inputFilePath);
  auto input = InputParser::parseAlignmentInput(inputLines);
  auto dnaFirst = DnaSequence(input.first);
  auto dnaSecond = DnaSequence(input.second);
  auto aligner = NeedlemanWunschAligner();
  return aligner.computeScore(dnaFirst.value(), dnaSecond.value(),
                              input.scoring);
}

AlignmentResult
AlignmentService::runFullAlignment(const std::string &inputFilePath) const {
  auto inputLines = FileReader::readLines(inputFilePath);
  auto input = InputParser::parseAlignmentInput(inputLines);
  auto dnaFirst = DnaSequence(input.first);
  auto dnaSecond = DnaSequence(input.second);
  auto aligner = NeedlemanWunschAligner();
  return aligner.align(dnaFirst.value(), dnaSecond.value(), input.scoring);
}