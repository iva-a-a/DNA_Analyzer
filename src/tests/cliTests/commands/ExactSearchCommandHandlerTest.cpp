#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "../../../cli/commands/ExactSearchCommandHandler.hpp"
#include "../../../core/common/errors/Errors.hpp"

namespace {

class ExactSearchCommandHandlerTest : public ::testing::Test {
protected:
  ExactSearchCommandHandler handler;
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

TEST_F(ExactSearchCommandHandlerTest, ThrowsWhenNoArguments) {
  const CommandLineOptions options{CommandType::ExactSearch, {}};

  EXPECT_THROW(handler.run(options), InputFormatError);
}

TEST_F(ExactSearchCommandHandlerTest, ThrowsWhenOneArgument) {
  const CommandLineOptions options{
      CommandType::ExactSearch,
      {"text.txt"},
  };

  EXPECT_THROW(handler.run(options), InputFormatError);
}

TEST_F(ExactSearchCommandHandlerTest, ThrowsWhenTooManyArguments) {
  const CommandLineOptions options{
      CommandType::ExactSearch,
      {"text.txt", "pattern.txt", "extra.txt"},
  };

  EXPECT_THROW(handler.run(options), InputFormatError);
}

TEST_F(ExactSearchCommandHandlerTest, PrintsPositionsAndReturnsZero) {
  const auto textPath = makeFilePath("exact_search_text.txt");
  const auto patternPath = makeFilePath("exact_search_pattern.txt");

  writeFile(textPath, "ACACAC");
  writeFile(patternPath, "ACA");

  const CommandLineOptions options{
      CommandType::ExactSearch,
      {textPath.string(), patternPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "0 2\n");
}

TEST_F(ExactSearchCommandHandlerTest, PrintsEmptyLineWhenNoMatches) {
  const auto textPath = makeFilePath("exact_search_no_match_text.txt");
  const auto patternPath = makeFilePath("exact_search_no_match_pattern.txt");

  writeFile(textPath, "AAAAAA");
  writeFile(patternPath, "CG");

  const CommandLineOptions options{
      CommandType::ExactSearch,
      {textPath.string(), patternPath.string()},
  };

  int exitCode = -1;
  const std::string output = captureStdout(options, exitCode);

  EXPECT_EQ(exitCode, 0);
  EXPECT_EQ(output, "\n");
}