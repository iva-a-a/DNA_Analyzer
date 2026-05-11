#pragma once

#include "ICommandHandler.hpp"

class AlignCommandHandler : public ICommandHandler {
public:
  int run(const CommandLineOptions &options) const override;
};