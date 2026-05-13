#include "RegexService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"
#include "../../core/regex/RegexMatcher.hpp"
#include "../../io/parser/InputParser.hpp"
#include "../../io/reader/FileReader.hpp"

bool RegexService::run(const std::string &inputFilePath) const {
  std::vector<std::string> inputLines = FileReader::readLines(inputFilePath);
  RegexMatchInput input = InputParser::parseRegexMatchInput(inputLines);

  auto dnaText = DnaSequence(input.sequence);
  auto matcher = RegexMatcher();

  return matcher.matches(dnaText.value(), input.pattern);
}