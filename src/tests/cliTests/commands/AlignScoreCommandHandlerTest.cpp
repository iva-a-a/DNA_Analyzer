#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "../../../cli/commands/AlignScoreCommandHandler.hpp"
#include "../../../core/common/errors/Errors.hpp"

namespace {

class AlignScoreCommandHandlerTest : public ::testing::Test {
protected:
  AlignScoreCommandHandler handler;
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
      exitCode = handler.run(options);
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

TEST_F(AlignScoreCommandHandlerTest, ThrowsWhenNoArguments) {
  const CommandLineOptions options{CommandType::AlignScore, {}};

  EXPECT_THROW(handler.run(options), InputFormatError);
}

TEST_F(AlignScoreCommandHandlerTest, ThrowsWhenTooManyArguments) {
  const CommandLineOptions options{
      CommandType::AlignScore,
      {"input.txt", "extra.txt"},
  };

  EXPECT_THROW(handler.run(options), InputFormatError);
}

TEST_F(AlignScoreCommandHandlerTest, PrintsScoreAndReturnsZero) {
  const auto inputPath = makeFilePath("align_score_command.txt");

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