#include "PatternParser.hpp"
#include "../../common/errors/Errors.hpp"
#include <cctype>

std::vector<PatternToken>
PatternParser::parse(const std::string &pattern) const {
  validateSyntax(pattern);

  std::vector<PatternToken> tokens;

  for (char c : pattern) {
    tokens.push_back(parseChar(c));
  }

  return tokens;
}

PatternToken PatternParser::parseChar(char c) const {
  if (std::isalpha(static_cast<unsigned char>(c))) {
    return {TokenType::Literal, c};
  } else if (c == '.') {
    return {TokenType::AnySingle, std::nullopt};
  } else if (c == '?') {
    return {TokenType::AnyOptional, std::nullopt};
  } else if (c == '+') {
    return {TokenType::RepeatPrevious, std::nullopt};
  } else if (c == '*') {
    return {TokenType::AnySequence, std::nullopt};
  }

  throw ValidationError("Invalid character in pattern");
}

bool PatternParser::isValidPatternChar(char c) const {
  return std::isalpha(static_cast<unsigned char>(c)) || c == '.' || c == '*' ||
         c == '?' || c == '+';
}

void PatternParser::validateSyntax(const std::string &pattern) const {
  if (pattern.empty()) {
    throw ValidationError("Pattern cannot be empty");
  }
  for (size_t i = 0; i < pattern.size(); ++i) {
    char c = pattern[i];
    if (!isValidPatternChar(c)) {
      throw ValidationError("Invalid character in pattern");
    }
    if (c == '+') {
      if (i == 0) {
        throw ValidationError(
            "'+' cannot be the first character in the pattern");
      }
      char prev = pattern[i - 1];
      if (prev == '*' || prev == '?' || prev == '+') {
        throw ValidationError("'+' cannot follow '*', '?', or another '+'");
      }
    }
  }
}