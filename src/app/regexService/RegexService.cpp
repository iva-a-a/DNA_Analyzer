#include "RegexService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"
#include "../../core/regex/RegexMatcher.hpp"
#include "../../io/parser/InputParser.hpp"
#include "../../io/reader/FileReader.hpp"

bool RegexService::run(const std::string &inputFilePath) const {
  auto inputLines = FileReader::readLines(inputFilePath);
  auto input = InputParser::parseRegexMatchInput(inputLines);
  auto dnaText = DnaSequence(input.sequence);
  auto dnaPattern = DnaSequence(input.pattern);
  auto matcher = RegexMatcher();
  return matcher.matches(dnaText.value(), dnaPattern.value());
}