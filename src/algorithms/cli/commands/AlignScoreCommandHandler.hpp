#pragma once

#include "ICommandHandler.hpp"

class AlignScoreCommandHandler : public ICommandHandler {
public:
  int run(const CommandLineOptions &options) const override;
};