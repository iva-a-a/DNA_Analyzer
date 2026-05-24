#include <gtest/gtest.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../cli/CommandLineParser.hpp"

namespace {

std::vector<char *> makeArgv(std::vector<std::string> &args) {
  std::vector<char *> argv;

  for (std::string &arg : args) {
    argv.push_back(arg.data());
  }

  return argv;
}

} // namespace

TEST(CommandLineParserTest, PrintUsageWritesUsageText) {
  std::stringstream output;

  CommandLineParser::printUsage("program", output);

  const std::string content = output.str();

  EXPECT_NE(content.find("Usage: program --input <csv> --output <report.md>"),
            std::string::npos);
  EXPECT_NE(content.find("Required:"), std::string::npos);
  EXPECT_NE(content.find("--input <path>"), std::string::npos);
  EXPECT_NE(content.find("--output <path>"), std::string::npos);
  EXPECT_NE(content.find("--min-k <n>"), std::string::npos);
  EXPECT_NE(content.find("--max-k <n>"), std::string::npos);
  EXPECT_NE(content.find("--seed <n>"), std::string::npos);
  EXPECT_NE(content.find("--kmeans-iters <n>"), std::string::npos);
  EXPECT_NE(content.find("--kmedoids-iters <n>"), std::string::npos);
  EXPECT_NE(content.find("--dbscan-min-points <n>"), std::string::npos);
  EXPECT_NE(content.find("--help"), std::string::npos);
}

TEST(CommandLineParserTest, ParseArgumentsParsesRequiredOptions) {
  std::vector<std::string> args = {
      "program", "--input", "dataset.csv", "--output", "report.md",
  };
  std::vector<char *> argv = makeArgv(args);

  const ClusteringConfig config = CommandLineParser::parseArguments(
      static_cast<int>(argv.size()), argv.data());

  EXPECT_EQ(config.inputPath, "dataset.csv");
  EXPECT_EQ(config.outputReportPath, "report.md");
}

TEST(CommandLineParserTest,
     ParseArgumentsKeepsDefaultValuesWhenOptionsMissing) {
  std::vector<std::string> args = {
      "program", "--input", "dataset.csv", "--output", "report.md",
  };
  std::vector<char *> argv = makeArgv(args);

  const ClusteringConfig config = CommandLineParser::parseArguments(
      static_cast<int>(argv.size()), argv.data());

  EXPECT_EQ(config.minKmerLength, 1);
  EXPECT_EQ(config.maxKmerLength, 6);
  EXPECT_EQ(config.randomSeed, 42);
  EXPECT_EQ(config.kmeansMaxIterations, 100);
  EXPECT_EQ(config.kmedoidsMaxIterations, 100);
  EXPECT_EQ(config.dbscanMinPoints, 3);
}

TEST(CommandLineParserTest, ParseArgumentsParsesAllOptions) {
  std::vector<std::string> args = {
      "program",   "--input",          "dataset.csv", "--output",
      "report.md", "--min-k",          "2",           "--max-k",
      "5",         "--seed",           "123",         "--kmeans-iters",
      "50",        "--kmedoids-iters", "70",          "--dbscan-min-points",
      "4",
  };
  std::vector<char *> argv = makeArgv(args);

  const ClusteringConfig config = CommandLineParser::parseArguments(
      static_cast<int>(argv.size()), argv.data());

  EXPECT_EQ(config.inputPath, "dataset.csv");
  EXPECT_EQ(config.outputReportPath, "report.md");
  EXPECT_EQ(config.minKmerLength, 2);
  EXPECT_EQ(config.maxKmerLength, 5);
  EXPECT_EQ(config.randomSeed, 123);
  EXPECT_EQ(config.kmeansMaxIterations, 50);
  EXPECT_EQ(config.kmedoidsMaxIterations, 70);
  EXPECT_EQ(config.dbscanMinPoints, 4);
}

TEST(CommandLineParserTest, ParseArgumentsAcceptsOptionsInDifferentOrder) {
  std::vector<std::string> args = {
      "program", "--max-k",     "4",       "--output", "report.md",
      "--input", "dataset.csv", "--min-k", "2",
  };
  std::vector<char *> argv = makeArgv(args);

  const ClusteringConfig config = CommandLineParser::parseArguments(
      static_cast<int>(argv.size()), argv.data());

  EXPECT_EQ(config.inputPath, "dataset.csv");
  EXPECT_EQ(config.outputReportPath, "report.md");
  EXPECT_EQ(config.minKmerLength, 2);
  EXPECT_EQ(config.maxKmerLength, 4);
}

TEST(CommandLineParserTest, ParseArgumentsThrowsWhenInputIsMissing) {
  std::vector<std::string> args = {
      "program",
      "--output",
      "report.md",
  };
  std::vector<char *> argv = makeArgv(args);

  EXPECT_THROW(CommandLineParser::parseArguments(static_cast<int>(argv.size()),
                                                 argv.data()),
               std::invalid_argument);
}

TEST(CommandLineParserTest, ParseArgumentsThrowsWhenOutputIsMissing) {
  std::vector<std::string> args = {
      "program",
      "--input",
      "dataset.csv",
  };
  std::vector<char *> argv = makeArgv(args);

  EXPECT_THROW(CommandLineParser::parseArguments(static_cast<int>(argv.size()),
                                                 argv.data()),
               std::invalid_argument);
}

TEST(CommandLineParserTest, ParseArgumentsThrowsWhenOptionIsUnknown) {
  std::vector<std::string> args = {
      "program", "--input", "dataset.csv", "--output", "report.md", "--unknown",
  };
  std::vector<char *> argv = makeArgv(args);

  EXPECT_THROW(CommandLineParser::parseArguments(static_cast<int>(argv.size()),
                                                 argv.data()),
               std::invalid_argument);
}

TEST(CommandLineParserTest, ParseArgumentsThrowsWhenOptionValueIsMissing) {
  std::vector<std::string> args = {
      "program",
      "--input",
      "dataset.csv",
      "--output",
  };
  std::vector<char *> argv = makeArgv(args);

  EXPECT_THROW(CommandLineParser::parseArguments(static_cast<int>(argv.size()),
                                                 argv.data()),
               std::invalid_argument);
}

TEST(CommandLineParserTest, ParseArgumentsThrowsWhenIntegerValueIsInvalid) {
  std::vector<std::string> args = {
      "program",   "--input", "dataset.csv", "--output",
      "report.md", "--min-k", "abc",
  };
  std::vector<char *> argv = makeArgv(args);

  EXPECT_THROW(CommandLineParser::parseArguments(static_cast<int>(argv.size()),
                                                 argv.data()),
               std::invalid_argument);
}

TEST(CommandLineParserTest, ParseArgumentsThrowsWhenMinKGreaterThanMaxK) {
  std::vector<std::string> args = {
      "program", "--input", "dataset.csv", "--output", "report.md",
      "--min-k", "5",       "--max-k",     "2",
  };
  std::vector<char *> argv = makeArgv(args);

  EXPECT_THROW(CommandLineParser::parseArguments(static_cast<int>(argv.size()),
                                                 argv.data()),
               std::invalid_argument);
}

TEST(CommandLineParserTest, ParseArgumentsAllowsEqualMinKAndMaxK) {
  std::vector<std::string> args = {
      "program", "--input", "dataset.csv", "--output", "report.md",
      "--min-k", "3",       "--max-k",     "3",
  };
  std::vector<char *> argv = makeArgv(args);

  const ClusteringConfig config = CommandLineParser::parseArguments(
      static_cast<int>(argv.size()), argv.data());

  EXPECT_EQ(config.minKmerLength, 3);
  EXPECT_EQ(config.maxKmerLength, 3);
}

TEST(CommandLineParserTest, ParseArgumentsExitsWhenHelpProvided) {
  std::vector<std::string> args = {
      "program",
      "--help",
  };
  std::vector<char *> argv = makeArgv(args);

  EXPECT_EXIT(CommandLineParser::parseArguments(static_cast<int>(argv.size()),
                                                argv.data()),
              ::testing::ExitedWithCode(0), "");
}