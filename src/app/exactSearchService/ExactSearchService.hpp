#pragma once

#include "../../core/rabinKarp/RabinKarpSearcher.hpp"
#include "../../core/rabinKarp/model/ExactSearchResult.hpp"
#include <string>

class ExactSearchService {
public:
  ExactSearchService() = default;

  ExactSearchResult search(const std::string &text,
                           const std::string &pattern) const;

private:
  RabinKarpSearcher _searcher;
};