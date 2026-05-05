#include "ExactSearchService.hpp"
#include "../../core/common/dnaSequence/DnaSequence.hpp"

ExactSearchResult ExactSearchService::search(const std::string &text,
                                             const std::string &pattern) const {
  auto dnaText = DnaSequence(text);
  auto dnaPattern = DnaSequence(pattern);

  auto positions = _searcher.find(dnaText.value(), dnaPattern.value());
  return ExactSearchResult{positions};
}