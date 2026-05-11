#pragma once

#include "ICommandHandler.hpp"

class RegexMatchCommandHandler : public ICommandHandler {
public:
  int run(const CommandLineOptions &options) const override;
};