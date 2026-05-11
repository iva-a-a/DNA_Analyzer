#pragma once

#include "../CommandLineOptions.hpp"

class ICommandHandler {
public:
  virtual ~ICommandHandler() = default;

  virtual int run(const CommandLineOptions &options) const = 0;
};