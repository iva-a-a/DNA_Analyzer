#include "AlignScoreCommandHandler.hpp"

#include <iostream>

#include "../../app/alignmentService/AlignmentService.hpp"
#include "../../io/formatter/OutputFormatter.hpp"
#include "CommandArguments.hpp"

int AlignScoreCommandHandler::run(const CommandLineOptions &options) const {
  CommandArgument::requireCount(
      options, 1,
      "Align score command requires exactly 1 argument: <inputFilePath>");

  const std::string &inputFilePath = options.arguments[0];

  AlignmentService service;
  int score = service.runScoreOnly(inputFilePath);

  std::cout << OutputFormatter::formatInt(score) << std::endl;

  return 0;
}