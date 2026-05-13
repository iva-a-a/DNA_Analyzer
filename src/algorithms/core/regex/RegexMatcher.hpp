#pragma once

#include <string>
#include <vector>

#include "models/PatternToken.hpp"

class RegexMatcher {
public:
  bool matches(const std::string &text, const std::string &pattern) const;

private:
  bool tokenMatches(char c, const PatternToken &token) const;
  bool calculateCell(size_t i, size_t j, const std::string &text,
                     const std::vector<PatternToken> &tokens,
                     const std::vector<std::vector<bool>> &dp) const;
};