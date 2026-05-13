#include "RegexMatchCommandHandler.hpp"

#include <iostream>

#include "../../app/regexService/RegexService.hpp"
#include "../../io/formatter/OutputFormatter.hpp"
#include "CommandArguments.hpp"

int RegexMatchCommandHandler::run(const CommandLineOptions &options) const {
  CommandArgument::requireCount(
      options, 1,
      "Regex match command requires exactly 1 argument: <inputFilePath>");

  const std::string &inputFilePath = options.arguments[0];

  RegexService service;
  bool result = service.run(inputFilePath);

  std::cout << OutputFormatter::formatBoolean(result) << std::endl;

  return 0;
}