#pragma once

#include <string>
#include <vector>

#include "CommandType.hpp"

struct CommandLineOptions {
  CommandType type;
  std::vector<std::string> arguments;
};