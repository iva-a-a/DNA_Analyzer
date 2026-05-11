#pragma once

#include "../../app/exactSearchService/ExactSearchService.hpp"
#include "ICommandHandler.hpp"

class ExactSearchCommandHandler : public ICommandHandler {
public:
  int run(const CommandLineOptions &options) const override;
};