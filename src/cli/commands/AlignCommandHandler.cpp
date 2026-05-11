#include "AlignCommandHandler.hpp"

#include <iostream>

#include "../../app/alignmentService/AlignmentService.hpp"
#include "../../io/formatter/OutputFormatter.hpp"
#include "CommandArguments.hpp"

int AlignCommandHandler::run(const CommandLineOptions &options) const {
  CommandArgument::requireCount(
      options, 1, "Align command requires exactly 1 argument: <inputFilePath>");

  const std::string &inputFilePath = options.arguments[0];

  AlignmentService service;
  AlignmentResult result = service.runFullAlignment(inputFilePath);

  std::cout << OutputFormatter::formatAlignment(result) << std::endl;

  return 0;
}