#pragma once

#include "ICommandHandler.hpp"

class MinWindowCommandHandler : public ICommandHandler {
public:
  int run(const CommandLineOptions &options) const override;
};