#pragma once

#include <string>

#include "../../core/common/errors/Errors.hpp"
#include "../CommandLineOptions.hpp"

namespace CommandArgument {

void requireCount(const CommandLineOptions &options, std::size_t expected,
                  const std::string &message);

} // namespace CommandArgument