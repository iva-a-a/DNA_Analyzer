#include "ExactSearchService.hpp"
#include "../../core/common/dnaSequence/DnaSequence.hpp"

ExactSearchResult ExactSearchService::search(const std::string &text,
                                             const std::string &pattern) const {
  DnaSequence dnaText(text);
  DnaSequence dnaPattern(pattern);

  auto positions =
      _searcher.find(dnaText.getSequence(), dnaPattern.getSequence());
  return ExactSearchResult{positions};
}