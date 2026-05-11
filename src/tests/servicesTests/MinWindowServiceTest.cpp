#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../app/minWindowService/MinWindowService.hpp"
#include "../../core/common/errors/Errors.hpp"

namespace {

class MinWindowServiceTest : public ::testing::Test {
protected:
  MinWindowService service;
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

  void TearDown() override {
    for (const auto &file : createdFiles) {
      std::error_code ec;
      std::filesystem::remove(file, ec);
    }
  }
};

} // namespace

TEST_F(MinWindowServiceTest, FindsMinimumWindow) {
  const auto path = makeFilePath("min_window_basic.txt");

  writeFile(path, "ACGTACGT\nGTA");

  EXPECT_EQ(service.run(path.string()), "GTA");
}

TEST_F(MinWindowServiceTest, ReturnsWholeStringWhenWholeStringIsMinimumWindow) {
  const auto path = makeFilePath("min_window_whole_string.txt");

  writeFile(path, "ACGT\nACGT");

  EXPECT_EQ(service.run(path.string()), "ACGT");
}

TEST_F(MinWindowServiceTest, ReturnsSingleCharacterWindow) {
  const auto path = makeFilePath("min_window_single_char.txt");

  writeFile(path, "ACGTACGT\nG");

  EXPECT_EQ(service.run(path.string()), "G");
}

TEST_F(MinWindowServiceTest, HandlesRepeatedTargetCharacters) {
  const auto path = makeFilePath("min_window_repeated_target.txt");

  writeFile(path, "AAACGT\nAAG");

  EXPECT_EQ(service.run(path.string()), "AACG");
}

TEST_F(MinWindowServiceTest, ReturnsEmptyStringWhenNoWindowExists) {
  const auto path = makeFilePath("min_window_no_window.txt");

  writeFile(path, "AAAAAA\nCG");

  EXPECT_EQ(service.run(path.string()), "");
}

TEST_F(MinWindowServiceTest, HandlesEqualSingleCharacterStrings) {
  const auto path = makeFilePath("min_window_equal_single_char.txt");

  writeFile(path, "A\nA");

  EXPECT_EQ(service.run(path.string()), "A");
}

TEST_F(MinWindowServiceTest, ThrowsWhenFileDoesNotExist) {
  const auto path = makeFilePath("min_window_missing.txt");

  std::error_code ec;
  std::filesystem::remove(path, ec);

  EXPECT_THROW(service.run(path.string()), FileReadError);
}

TEST_F(MinWindowServiceTest, ThrowsWhenInputHasNoLines) {
  const auto path = makeFilePath("min_window_no_lines.txt");

  writeFile(path, "");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(MinWindowServiceTest, ThrowsWhenInputHasOneLine) {
  const auto path = makeFilePath("min_window_one_line.txt");

  writeFile(path, "ACGT");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(MinWindowServiceTest, ThrowsWhenInputHasTooManyLines) {
  const auto path = makeFilePath("min_window_too_many_lines.txt");

  writeFile(path, "ACGT\nAC\nextra");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(MinWindowServiceTest, AllowsEmptyStrings) {
  const auto path = makeFilePath("min_window_empty_strings.txt");

  writeFile(path, "\n\n");

  EXPECT_EQ(service.run(path.string()), "");
}

TEST_F(MinWindowServiceTest, ThrowsWhenSourceContainsInvalidDnaCharacters) {
  const auto path = makeFilePath("min_window_invalid_source.txt");

  writeFile(path, "ACBX\nAC");

  EXPECT_THROW(service.run(path.string()), ValidationError);
}

TEST_F(MinWindowServiceTest, ThrowsWhenTargetContainsInvalidDnaCharacters) {
  const auto path = makeFilePath("min_window_invalid_target.txt");

  writeFile(path, "ACGT\nAX");

  EXPECT_THROW(service.run(path.string()), ValidationError);
}