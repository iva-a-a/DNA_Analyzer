#pragma once

#include <string>

namespace Validation {
bool isDnaChar(char ch) noexcept;
bool isDnaString(const std::string &value) noexcept;
} // namespace Validation
