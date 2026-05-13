#include "MinWindowCommandHandler.hpp"

#include <iostream>

#include "../../app/minWindowService/MinWindowService.hpp"
#include "CommandArguments.hpp"

int MinWindowCommandHandler::run(const CommandLineOptions &options) const {
  CommandArgument::requireCount(
      options, 1,
      "Min window command requires exactly 1 argument: <inputFilePath>");

  const std::string &inputFilePath = options.arguments[0];

  MinWindowService service;
  std::string result = service.run(inputFilePath);

  std::cout << result << std::endl;

  return 0;
}
