#pragma once

#include <string>

namespace Validation {
bool IsDnaChar(char ch) noexcept;
bool IsDnaString(const std::string &value) noexcept;
} // namespace Validation
