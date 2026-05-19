#include "KmerExtractor.hpp"

#include "../common/Errors.hpp"

namespace KmerExtractor {

std::vector<std::string> extractKmers(const std::string &sequence,
                                      std::size_t k) {
  if (k <= 0) {
    throw ValidationError("k-mer size must be greater than zero");
  }
  if (k > sequence.size()) {
    return {};
  }
  std::vector<std::string> kmers;

  for (std::size_t i = 0; i <= sequence.size() - k; ++i) {
    kmers.push_back(sequence.substr(i, k));
  }
  return kmers;
}

} // namespace KmerExtractor
