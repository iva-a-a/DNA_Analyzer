#pragma once

#include <string>
#include <vector>

#include "../models/PatternToken.hpp"

class PatternParser {
public:
  std::vector<PatternToken> parse(const std::string &pattern) const;

private:
  PatternToken parseChar(char c) const;
  void validateSyntax(const std::string &pattern) const;
  bool isValidPatternChar(char c) const;
};