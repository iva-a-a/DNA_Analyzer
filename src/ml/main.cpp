#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "app/ClusteringConfig.hpp"
#include "app/ClusteringPipeline.hpp"
#include "common/Errors.hpp"

namespace {

void printUsage(const char *programName) {
  std::cerr << "Usage: " << programName
            << " --input <csv> --output <report.md> [options]\n\n"
            << "Required:\n"
            << "  --input <path>           Input dataset CSV file\n"
            << "  --output <path>          Output Markdown report file\n\n"
            << "Optional:\n"
            << "  --min-k <n>              Minimum k-mer length (default: 1)\n"
            << "  --max-k <n>              Maximum k-mer length (default: 6)\n"
            << "  --seed <n>               Random seed (default: 42)\n"
            << "  --kmeans-iters <n>       KMeans max iterations (default: "
               "100)\n"
            << "  --kmedoids-iters <n>     KMedoids max iterations (default: "
               "100)\n"
            << "  --dbscan-eps <value>     DBSCAN epsilon (default: 0.8)\n"
            << "  --dbscan-min-points <n>  DBSCAN min points (default: 2)\n"
            << "  --help                   Show this help message\n\n"
            << "Example:\n"
            << "  " << programName
            << " --input datasets/clustering/dna_dataset.csv --output "
               "task6_report.md\n";
}

int parseInt(const std::string &value, const std::string &optionName) {
  try {
    const int parsed = std::stoi(value);
    return parsed;
  } catch (const std::exception &) {
    throw std::invalid_argument("Invalid value for " + optionName + ": " +
                                value);
  }
}

// double parseDouble(const std::string &value, const std::string &optionName) {
//   try {
//     const double parsed = std::stod(value);
//     return parsed;
//   } catch (const std::exception &) {
//     throw std::invalid_argument("Invalid value for " + optionName + ": " +
//                                 value);
//   }
// }

bool requiresValue(const std::string &arg) {
  return arg == "--input" || arg == "--output" || arg == "--min-k" ||
         arg == "--max-k" || arg == "--seed" || arg == "--kmeans-iters" ||
         arg == "--kmedoids-iters" || arg == "--dbscan-eps" ||
         arg == "--dbscan-min-points";
}

ClusteringConfig parseArguments(int argc, char *argv[]) {
  ClusteringConfig config;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];

    if (arg == "--help") {
      printUsage(argv[0]);
      std::exit(0);
    }

    if (!requiresValue(arg)) {
      throw std::invalid_argument("Unknown option: " + arg);
    }

    if (i + 1 >= argc) {
      throw std::invalid_argument("Missing value for option: " + arg);
    }

    if (arg == "--input") {
      config.inputPath = argv[++i];
    } else if (arg == "--output") {
      config.outputReportPath = argv[++i];
    } else if (arg == "--min-k") {
      config.minKmerLength = parseInt(argv[++i], arg);
    } else if (arg == "--max-k") {
      config.maxKmerLength = parseInt(argv[++i], arg);
    } else if (arg == "--seed") {
      config.randomSeed = parseInt(argv[++i], arg);
    } else if (arg == "--kmeans-iters") {
      config.kmeansMaxIterations = parseInt(argv[++i], arg);
    } else if (arg == "--kmedoids-iters") {
      config.kmedoidsMaxIterations = parseInt(argv[++i], arg);
    } /*else if (arg == "--dbscan-eps") {
      config.dbscanEps = parseDouble(argv[++i], arg);
  }*/
    else if (arg == "--dbscan-min-points") {
      config.dbscanMinPoints = parseInt(argv[++i], arg);
    }
  }

  if (config.inputPath.empty()) {
    throw std::invalid_argument("Missing required option: --input");
  }

  if (config.outputReportPath.empty()) {
    throw std::invalid_argument("Missing required option: --output");
  }

  if (config.minKmerLength > config.maxKmerLength) {
    throw std::invalid_argument("min-k must be less than or equal to max-k");
  }

  return config;
}

} // namespace

int main(int argc, char *argv[]) {
  try {
    const ClusteringConfig config = parseArguments(argc, argv);
    ClusteringPipeline pipeline;
    pipeline.run(config);
    std::cout << "Report written to: " << config.outputReportPath << '\n';
    return 0;
  } catch (const MlPipelineError &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  } catch (const std::invalid_argument &error) {
    std::cerr << "Error: " << error.what() << '\n';
    printUsage(argv[0]);
    return 1;
  }
}
