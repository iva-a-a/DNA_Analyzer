#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "app/ClusteringConfig.hpp"
#include "app/ClusteringPipeline.hpp"
#include "cli/CommandLineParser.hpp"
#include "common/Errors.hpp"

int main(int argc, char *argv[]) {
  try {
    const ClusteringConfig config =
        CommandLineParser::parseArguments(argc, argv);
    ClusteringPipeline pipeline;
    pipeline.run(config);
    std::cout << "Report written to: " << config.outputReportPath << '\n';
    return 0;
  } catch (const MlPipelineError &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  } catch (const std::invalid_argument &error) {
    std::cerr << "Error: " << error.what() << '\n';
    CommandLineParser::printUsage(argv[0], std::cerr);
    return 1;
  }
}
