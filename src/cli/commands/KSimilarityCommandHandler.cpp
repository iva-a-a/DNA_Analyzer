#include "KSimilarityCommandHandler.hpp"

#include <iostream>

#include "../../app/kSimilarityService/KSimilarityService.hpp"
#include "../../io/formatter/OutputFormatter.hpp"
#include "CommandArguments.hpp"

int KSimilarityCommandHandler::run(const CommandLineOptions &options) const {
  CommandArgument::requireCount(
      options, 1,
      "K-similarity command requires exactly 1 argument: <inputFilePath>");

  const std::string &inputFilePath = options.arguments[0];

  KSimilarityService service;
  int result = service.run(inputFilePath);

  std::cout << OutputFormatter::formatInt(result) << std::endl;

  return 0;
}