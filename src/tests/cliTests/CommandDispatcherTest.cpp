#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../cli/dispatcher/CommandDispatcher.hpp"
#include "../../core/common/errors/Errors.hpp"

namespace {

class CommandDispatcherTest : public ::testing::Test {
protected:
  CommandDispatcher dispatcher;
  std::vector<std::filesystem::path> createdFiles;

  std::filesystem::path makeFilePath(const std::string &name) {
    auto path = std::filesystem::temp_directory_path() / name;
    createdFiles.push_back(path);
    return path;
  }

  void writeFile(const std::filesystem::path &path,
                 const std::string &content) {
    std::ofstream file(path);
    ASSERT_TRUE(file.is_open());
    file << content;
  }

  std::string captureStdout(const CommandLineOptions &options, int &exitCode) {
    std::ostringstream captured;
    auto *oldBuffer = std::cout.rdbuf(captured.rdbuf());

    try {
      exitCode = dispatcher.dispatch(options);
      std::cout.rdbuf(oldBuffer);
    } catch (...) {
      std::cout.rdbuf(oldBuffer);
      throw;
    }

    return captured.str();
  }

  void TearDown() override {
    for (const auto &file : createdFiles) {
      std::error_code ec;
      std::filesystem::remove(file, ec);
    }
  }
};

} // namespace

TEST_F(CommandDispatcherTest, ThrowsOnUnknownCommand) {
  const CommandLineOptions options{CommandType::Unknown, {}};

  EXPECT_THROW(dispatcher.dispatch(options), InputFormatError);
}

TEST_F(CommandDispatcherTest, DispatchesExactSearchCommand) {
  const auto textPath = makeFilePath("dispatcher_exact_text.txt");
  const auto patternPath = makeFilePath("dispatcher_exact_pattern.txt");

  writeFile(textPath, "ACACAC");
  writeFile(patternPath, "ACA");

  const CommandLineOptions options{
      CommandType::ExactSearch,
      {textPath.string(), patternPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "1 3\n");
}

TEST_F(CommandDispatcherTest, DispatchesAlignScoreCommand) {
  const auto inputPath = makeFilePath("dispatcher_align_score.txt");

  writeFile(inputPath, "1 -1 -2\nAC\nAC");

  const CommandLineOptions options{
      CommandType::AlignScore,
      {inputPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "2\n");
}

TEST_F(CommandDispatcherTest, DispatchesAlignCommand) {
  const auto inputPath = makeFilePath("dispatcher_align.txt");

  writeFile(inputPath, "1 -1 -2\nAC\nAC");

  const CommandLineOptions options{
      CommandType::Align,
      {inputPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "2\nAC\n||\nAC\n");
}

TEST_F(CommandDispatcherTest, DispatchesRegexMatchCommand) {
  const auto inputPath = makeFilePath("dispatcher_regex_true.txt");

  writeFile(inputPath, "TT\nT*");

  const CommandLineOptions options{
      CommandType::RegexMatch,
      {inputPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "True\n");
}

TEST_F(CommandDispatcherTest, DispatchesKSimilarityCommand) {
  const auto inputPath = makeFilePath("dispatcher_k_similarity.txt");

  writeFile(inputPath, "AC\nCA");

  const CommandLineOptions options{
      CommandType::KSimilarity,
      {inputPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "1\n");
}

TEST_F(CommandDispatcherTest, DispatchesMinWindowCommand) {
  const auto inputPath = makeFilePath("dispatcher_min_window.txt");

  writeFile(inputPath, "ACGTACGT\nGTA");

  const CommandLineOptions options{
      CommandType::MinWindow,
      {inputPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "GTA\n");
}