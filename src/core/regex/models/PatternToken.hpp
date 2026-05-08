#pragma once

#include "TokenType.hpp"

#include <optional>

struct PatternToken {
  TokenType type;
  std::optional<char> value;
};