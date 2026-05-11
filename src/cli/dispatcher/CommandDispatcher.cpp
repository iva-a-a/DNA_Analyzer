#include "CommandDispatcher.hpp"

#include <iostream>

#include "../../app/alignmentService/AlignmentService.hpp"
#include "../../app/exactSearchService/ExactSearchService.hpp"
#include "../../app/kSimilarityService/KSimilarityService.hpp"
#include "../../app/minWindowService/MinWindowService.hpp"
#include "../../app/regexService/RegexService.hpp"
#include "../../core/common/errors/Errors.hpp"
#include "../../io/formatter/OutputFormatter.hpp"

int CommandDispatcher::dispatch(const CommandLineOptions &options) const {
  switch (options.type) {
  case CommandType::ExactSearch:
    return runExactSearch(options);
  case CommandType::AlignScore:
    return runAlignScore(options);
  case CommandType::Align:
    return runAlign(options);
  case CommandType::RegexMatch:
    return runRegexMatch(options);
  case CommandType::KSimilarity:
    return runKSimilarity(options);
  case CommandType::MinWindow:
    return runMinWindow(options);
  default:
    throw InputFormatError("Unknown command");
  }
}

int CommandDispatcher::runExactSearch(const CommandLineOptions &options) const {
  if (options.arguments.size() != 2) {
    throw InputFormatError("Exact search command requires exactly 2 arguments: "
                           "<textFilePath> <patternFilePath>");
  }

  const std::string &textFilePath = options.arguments[0];
  const std::string &patternFilePath = options.arguments[1];
  ExactSearchService service;

  ExactSearchResult result = service.run(textFilePath, patternFilePath);

  std::cout << OutputFormatter::formatPositions(result) << std::endl;

  return 0;
}

int CommandDispatcher::runAlignScore(const CommandLineOptions &options) const {
  if (options.arguments.size() != 1) {
    throw InputFormatError(
        "Align score command requires exactly 1 argument: <inputFilePath>");
  }

  const std::string &inputFilePath = options.arguments[0];
  AlignmentService service;

  int score = service.runScoreOnly(inputFilePath);

  std::cout << OutputFormatter::formatInt(score) << std::endl;

  return 0;
}

int CommandDispatcher::runAlign(const CommandLineOptions &options) const {
  if (options.arguments.size() != 1) {
    throw InputFormatError(
        "Align command requires exactly 1 argument: <inputFilePath>");
  }

  const std::string &inputFilePath = options.arguments[0];
  AlignmentService service;

  AlignmentResult result = service.runFullAlignment(inputFilePath);

  std::cout << OutputFormatter::formatAlignment(result) << std::endl;

  return 0;
}

int CommandDispatcher::runRegexMatch(const CommandLineOptions &options) const {
  if (options.arguments.size() != 1) {
    throw InputFormatError("Regex match command requires exactly 1 argument: "
                           "<inputFilePath>");
  }

  const std::string &textFilePath = options.arguments[0];
  RegexService service;

  bool result = service.run(textFilePath);

  std::cout << OutputFormatter::formatBoolean(result) << std::endl;

  return 0;
}

int CommandDispatcher::runKSimilarity(const CommandLineOptions &options) const {
  if (options.arguments.size() != 1) {
    throw InputFormatError(
        "K-similarity command requires exactly 1 argument: <inputFilePath>");
  }

  const std::string &inputFilePath = options.arguments[0];
  KSimilarityService service;

  int result = service.run(inputFilePath);

  std::cout << OutputFormatter::formatInt(result) << std::endl;

  return 0;
}

int CommandDispatcher::runMinWindow(const CommandLineOptions &options) const {
  if (options.arguments.size() != 1) {
    throw InputFormatError(
        "Min window command requires exactly 1 argument: <inputFilePath>");
  }

  const std::string &inputFilePath = options.arguments[0];
  MinWindowService service;

  std::string result = service.run(inputFilePath);

  std::cout << result << std::endl;

  return 0;
}