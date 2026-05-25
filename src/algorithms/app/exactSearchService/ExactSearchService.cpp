#include "ExactSearchService.hpp"

#include <vector>

#include "../../core/common/dnaSequence/DnaSequence.hpp"
#include "../../core/rabinKarp/RabinKarpSearcher.hpp"
#include "../../io/parser/InputParser.hpp"
#include "../../io/reader/FileReader.hpp"

ExactSearchResult
ExactSearchService::run(const std::string &textFilePath,
                        const std::string &patternFilePath) const {
  std::vector<std::string> textLines = FileReader::readLines(textFilePath);
  std::vector<std::string> patternLines =
      FileReader::readLines(patternFilePath);

  ExactSearchInput input =
      InputParser::parseExactSearchInput(textLines, patternLines);

  auto dnaText = DnaSequence(input.text);
  auto dnaPattern = DnaSequence(input.pattern);
  auto searcher = RabinKarpSearcher();
  ExactSearchResult result;

  result.positions = searcher.find(dnaText.value(), dnaPattern.value());

  return result;
}