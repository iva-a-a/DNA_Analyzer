#include "CommandDispatcher.hpp"

#include <iostream>

#include "../../core/common/errors/Errors.hpp"
#include "../commands/AlignCommandHandler.hpp"
#include "../commands/AlignScoreCommandHandler.hpp"
#include "../commands/ExactSearchCommandHandler.hpp"
#include "../commands/KSimilarityCommandHandler.hpp"
#include "../commands/MinWindowCommandHandler.hpp"
#include "../commands/RegexMatchCommandHandler.hpp"

CommandDispatcher::CommandDispatcher() {
  _handlers.emplace(CommandType::ExactSearch,
                    std::make_unique<ExactSearchCommandHandler>());

  _handlers.emplace(CommandType::AlignScore,
                    std::make_unique<AlignScoreCommandHandler>());

  _handlers.emplace(CommandType::Align,
                    std::make_unique<AlignCommandHandler>());

  _handlers.emplace(CommandType::RegexMatch,
                    std::make_unique<RegexMatchCommandHandler>());

  _handlers.emplace(CommandType::KSimilarity,
                    std::make_unique<KSimilarityCommandHandler>());

  _handlers.emplace(CommandType::MinWindow,
                    std::make_unique<MinWindowCommandHandler>());
}

int CommandDispatcher::dispatch(const CommandLineOptions &options) const {
  std::unordered_map<CommandType,
                     std::unique_ptr<ICommandHandler>>::const_iterator it =
      _handlers.find(options.type);

  if (it == _handlers.end()) {
    throw InputFormatError("Unknown command");
  }

  return it->second->run(options);
}
