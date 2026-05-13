#pragma once

#include "ICommandHandler.hpp"

class ExactSearchCommandHandler : public ICommandHandler {
public:
  int run(const CommandLineOptions &options) const override;
};