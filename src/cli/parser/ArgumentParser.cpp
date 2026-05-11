#include "ArgumentParser.hpp"

namespace {

CommandType parseCommand(const std::string &command) {
  if (command == "exact-search") {
    return CommandType::ExactSearch;
  } else if (command == "align-score") {
    return CommandType::AlignScore;
  } else if (command == "align") {
    return CommandType::Align;
  } else if (command == "regex-match") {
    return CommandType::RegexMatch;
  } else if (command == "k-similarity") {
    return CommandType::KSimilarity;
  } else if (command == "min-window") {
    return CommandType::MinWindow;
  }
  return CommandType::Unknown;
}

} // namespace

namespace ArgumentParser {

CommandLineOptions parse(int argc, char *argv[]) {
  if (argc < 2) {
    return {CommandType::Unknown, {}};
  }

  CommandType type = parseCommand(argv[1]);
  std::vector<std::string> arguments;

  for (int i = 2; i < argc; ++i) {
    arguments.push_back(argv[i]);
  }

  return {type, arguments};
}

} // namespace ArgumentParser