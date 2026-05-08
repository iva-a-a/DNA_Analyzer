#include "ExactSearchService.hpp"

#include "../../core/common/dnaSequence/DnaSequence.hpp"
#include "../../core/rabinKarp/RabinKarpSearcher.hpp"
#include "../../io/parser/InputParser.hpp"
#include "../../io/reader/FileReader.hpp"

ExactSearchResult
ExactSearchService::run(const std::string &textFilePath,
                        const std::string &patternFilePath) const {
  auto textLines = FileReader::readLine(textFilePath);
  auto patternLines = FileReader::readLine(patternFilePath);
  auto input = InputParser::parseExactSearchInput(textLines, patternLines);
  auto dnaText = DnaSequence(input.text);
  auto dnaPattern = DnaSequence(input.pattern);
  auto searcher = RabinKarpSearcher();
  ExactSearchResult result;
  result.positions = searcher.find(dnaText.value(), dnaPattern.value());

  return result;
}