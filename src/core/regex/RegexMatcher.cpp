#include "RegexMatcher.hpp"

#include "parser/PatternParser.hpp"

bool RegexMatcher::matches(const std::string &text,
                           const std::string &pattern) const {
  PatternParser parser;
  std::vector<PatternToken> tokens = parser.parse(pattern);

  const size_t n = text.size();
  const size_t m = tokens.size();

  std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(m + 1, false));
  dp[0][0] = true;

  for (size_t i = 0; i <= n; ++i) {
    for (size_t j = 1; j <= m; ++j) {
      dp[i][j] = calculateCell(i, j, text, tokens, dp);
    }
  }

  return dp[n][m];
}

bool RegexMatcher::calculateCell(
    size_t i, size_t j, const std::string &text,
    const std::vector<PatternToken> &tokens,
    const std::vector<std::vector<bool>> &dp) const {
  const PatternToken &token = tokens[j - 1];

  switch (token.type) {
  case TokenType::Literal:
    return i > 0 && tokenMatches(text[i - 1], token) && dp[i - 1][j - 1];

  case TokenType::AnySingle:
    return i > 0 && dp[i - 1][j - 1];

  case TokenType::AnyOptional:
    return dp[i][j - 1] || (i > 0 && dp[i - 1][j - 1]);

  case TokenType::AnySequence:
    return dp[i][j - 1] || (i > 0 && dp[i - 1][j]);

  case TokenType::RepeatPrevious:
    if (j <= 1) {
      return false;
    }

    const auto &prevToken = tokens[j - 2];

    return dp[i][j - 1] ||
           (i > 0 && tokenMatches(text[i - 1], prevToken) && dp[i - 1][j]);
  }

  return false;
}

bool RegexMatcher::tokenMatches(char c, const PatternToken &token) const {
  if (token.type == TokenType::Literal) {
    return token.value.has_value() && token.value.value() == c;
  }

  if (token.type == TokenType::AnySingle) {
    return true;
  }

  return false;
}