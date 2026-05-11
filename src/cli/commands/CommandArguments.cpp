#include "CommandArguments.hpp"

namespace CommandArgument {

void requireCount(const CommandLineOptions &options, std::size_t expected,
                  const std::string &message) {
  if (options.arguments.size() != expected) {
    throw InputFormatError(message);
  }
}

} // namespace CommandArgument
