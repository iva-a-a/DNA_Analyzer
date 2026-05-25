#pragma once

#include <string>

class DnaRegexPattern {
public:
  explicit DnaRegexPattern(const std::string &value);

  const std::string &value() const noexcept;

private:
  std::string _value;
};