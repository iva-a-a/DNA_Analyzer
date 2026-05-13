#pragma once

#include <string>

#include "../../core/rabinKarp/model/ExactSearchResult.hpp"

class ExactSearchService {
public:
  ExactSearchResult run(const std::string &textFilePath,
                        const std::string &patternFilePath) const;
};