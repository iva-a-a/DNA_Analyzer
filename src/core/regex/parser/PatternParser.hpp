#pragma once

#include "../models/PatternToken.hpp"
#include <string>
#include <vector>

class PatternParser {
public:
  std::vector<PatternToken> parse(const std::string &pattern) const;

private:
  PatternToken parseChar(char c) const;
  void validateSyntax(const std::string &pattern) const;
  bool isValidPatternChar(char c) const;
};