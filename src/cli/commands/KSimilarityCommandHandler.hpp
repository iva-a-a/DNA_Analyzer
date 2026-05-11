#pragma once

#include "ICommandHandler.hpp"

class KSimilarityCommandHandler : public ICommandHandler {
public:
  int run(const CommandLineOptions &options) const override;
};