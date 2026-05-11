#include "ExactSearchCommandHandler.hpp"

#include <iostream>

#include "../../app/exactSearchService/ExactSearchService.hpp"
#include "../../io/formatter/OutputFormatter.hpp"
#include "CommandArguments.hpp"

int ExactSearchCommandHandler::run(const CommandLineOptions &options) const {
  CommandArgument::requireCount(
      options, 2,
      "Exact search command requires exactly 2 arguments: "
      "<textFilePath> <patternFilePath>");

  const std::string &textFilePath = options.arguments[0];
  const std::string &patternFilePath = options.arguments[1];

  ExactSearchService service;
  ExactSearchResult result = service.run(textFilePath, patternFilePath);

  std::cout << OutputFormatter::formatPositions(result) << std::endl;

  return 0;
}