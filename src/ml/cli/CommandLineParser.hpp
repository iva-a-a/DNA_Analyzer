#pragma once

#include <ostream>

#include "../app/ClusteringConfig.hpp"

namespace CommandLineParser {

void printUsage(const char *programName, std::ostream &output);
ClusteringConfig parseArguments(int argc, char *argv[]);

} // namespace CommandLineParser