#pragma once

#include <string>
#include <vector>

namespace KmerExtractor {

std::vector<std::string> extractKmers(const std::string &sequence,
                                      std::size_t k);

} // namespace KmerExtractor