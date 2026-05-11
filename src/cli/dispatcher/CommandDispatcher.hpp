#pragma once

#include <memory>
#include <unordered_map>

#include "../CommandLineOptions.hpp"
#include "../commands/ICommandHandler.hpp"

class CommandDispatcher {
public:
  CommandDispatcher();
  int dispatch(const CommandLineOptions &options) const;

private:
  std::unordered_map<CommandType, std::unique_ptr<ICommandHandler>> _handlers;
};