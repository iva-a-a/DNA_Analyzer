#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "../../../cli/commands/MinWindowCommandHandler.hpp"
#include "../../../core/common/errors/Errors.hpp"

namespace {

class MinWindowCommandHandlerTest : public ::testing::Test {
protected:
  MinWindowCommandHandler handler;
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

TEST_F(MinWindowCommandHandlerTest, ThrowsWhenNoArguments) {
  const CommandLineOptions options{CommandType::MinWindow, {}};

  EXPECT_THROW(handler.run(options), InputFormatError);
}

TEST_F(MinWindowCommandHandlerTest, ThrowsWhenTooManyArguments) {
  const CommandLineOptions options{
      CommandType::MinWindow,
      {"input.txt", "extra.txt"},
  };

  EXPECT_THROW(handler.run(options), InputFormatError);
}

TEST_F(MinWindowCommandHandlerTest, PrintsResultAndReturnsZero) {
  const auto inputPath = makeFilePath("min_window_command.txt");

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