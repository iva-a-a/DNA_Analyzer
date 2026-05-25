#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../app/exactSearchService/ExactSearchService.hpp"
#include "../../core/common/errors/Errors.hpp"

namespace {

class ExactSearchServiceTest : public ::testing::Test {
protected:
  ExactSearchService service;
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

TEST_F(ExactSearchServiceTest, FindsSingleMatch) {
  const auto textPath = makeFilePath("exact_text_single.txt");
  const auto patternPath = makeFilePath("exact_pattern_single.txt");

  writeFile(textPath, "ACGTACGT");
  writeFile(patternPath, "GTA");

  const ExactSearchResult result =
      service.run(textPath.string(), patternPath.string());

  const std::vector<std::size_t> expected = {2};
  EXPECT_EQ(result.positions, expected);
}

TEST_F(ExactSearchServiceTest, FindsMultipleMatches) {
  const auto textPath = makeFilePath("exact_text_multiple.txt");
  const auto patternPath = makeFilePath("exact_pattern_multiple.txt");

  writeFile(textPath, "ACACAC");
  writeFile(patternPath, "ACA");

  const ExactSearchResult result =
      service.run(textPath.string(), patternPath.string());

  const std::vector<std::size_t> expected = {0, 2};
  EXPECT_EQ(result.positions, expected);
}

TEST_F(ExactSearchServiceTest, FindsOverlappingMatches) {
  const auto textPath = makeFilePath("exact_text_overlap.txt");
  const auto patternPath = makeFilePath("exact_pattern_overlap.txt");

  writeFile(textPath, "AAAA");
  writeFile(patternPath, "AA");

  const ExactSearchResult result =
      service.run(textPath.string(), patternPath.string());

  const std::vector<std::size_t> expected = {0, 1, 2};
  EXPECT_EQ(result.positions, expected);
}

TEST_F(ExactSearchServiceTest, ReturnsEmptyWhenNoMatches) {
  const auto textPath = makeFilePath("exact_text_no_match.txt");
  const auto patternPath = makeFilePath("exact_pattern_no_match.txt");

  writeFile(textPath, "AAAAAA");
  writeFile(patternPath, "CG");

  const ExactSearchResult result =
      service.run(textPath.string(), patternPath.string());

  EXPECT_TRUE(result.positions.empty());
}

TEST_F(ExactSearchServiceTest, FindsMatchWhenPatternEqualsText) {
  const auto textPath = makeFilePath("exact_text_equal.txt");
  const auto patternPath = makeFilePath("exact_pattern_equal.txt");

  writeFile(textPath, "ACGT");
  writeFile(patternPath, "ACGT");

  const ExactSearchResult result =
      service.run(textPath.string(), patternPath.string());

  const std::vector<std::size_t> expected = {0};
  EXPECT_EQ(result.positions, expected);
}

TEST_F(ExactSearchServiceTest, FindsSingleCharacterPattern) {
  const auto textPath = makeFilePath("exact_text_single_char.txt");
  const auto patternPath = makeFilePath("exact_pattern_single_char.txt");

  writeFile(textPath, "ACGTA");
  writeFile(patternPath, "A");

  const ExactSearchResult result =
      service.run(textPath.string(), patternPath.string());

  const std::vector<std::size_t> expected = {0, 4};
  EXPECT_EQ(result.positions, expected);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenTextFileHasMoreThanOneLine) {
  const auto textPath = makeFilePath("exact_text_multiline.txt");
  const auto patternPath = makeFilePath("exact_pattern_singleline.txt");

  writeFile(textPath, "ACGT\nAAAA");
  writeFile(patternPath, "CG");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               InputFormatError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenPatternFileHasMoreThanOneLine) {
  const auto textPath = makeFilePath("exact_text_singleline.txt");
  const auto patternPath = makeFilePath("exact_pattern_multiline.txt");

  writeFile(textPath, "ACGT");
  writeFile(patternPath, "CG\nTT");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               InputFormatError);
}

TEST_F(ExactSearchServiceTest, SupportsMaximumAllowedTextAndPatternLengths) {
  const auto textPath = makeFilePath("exact_text_max.txt");
  const auto patternPath = makeFilePath("exact_pattern_max.txt");

  const std::string text(10000, 'A');
  const std::string pattern(100, 'A');

  writeFile(textPath, text);
  writeFile(patternPath, pattern);

  const ExactSearchResult result =
      service.run(textPath.string(), patternPath.string());

  ASSERT_FALSE(result.positions.empty());
  EXPECT_EQ(result.positions.front(), 0u);
  EXPECT_EQ(result.positions.back(), 9900u);
  EXPECT_EQ(result.positions.size(), 9901u);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenTextFileDoesNotExist) {
  const auto textPath = makeFilePath("exact_missing_text.txt");
  const auto patternPath = makeFilePath("exact_existing_pattern.txt");

  std::error_code ec;
  std::filesystem::remove(textPath, ec);
  writeFile(patternPath, "A");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               FileReadError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenPatternFileDoesNotExist) {
  const auto textPath = makeFilePath("exact_existing_text.txt");
  const auto patternPath = makeFilePath("exact_missing_pattern.txt");

  writeFile(textPath, "A");

  std::error_code ec;
  std::filesystem::remove(patternPath, ec);

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               FileReadError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenTextIsEmpty) {
  const auto textPath = makeFilePath("exact_empty_text.txt");
  const auto patternPath = makeFilePath("exact_pattern_for_empty_text.txt");

  writeFile(textPath, "");
  writeFile(patternPath, "A");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               InputFormatError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenPatternIsEmpty) {
  const auto textPath = makeFilePath("exact_text_for_empty_pattern.txt");
  const auto patternPath = makeFilePath("exact_empty_pattern.txt");

  writeFile(textPath, "ACGT");
  writeFile(patternPath, "");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               InputFormatError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenTextTooLong) {
  const auto textPath = makeFilePath("exact_text_too_long.txt");
  const auto patternPath = makeFilePath("exact_pattern_valid.txt");

  writeFile(textPath, std::string(10001, 'A'));
  writeFile(patternPath, "A");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               InputFormatError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenPatternTooLong) {
  const auto textPath = makeFilePath("exact_text_valid.txt");
  const auto patternPath = makeFilePath("exact_pattern_too_long.txt");

  writeFile(textPath, std::string(200, 'A'));
  writeFile(patternPath, std::string(101, 'A'));

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               InputFormatError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenPatternLongerThanText) {
  const auto textPath = makeFilePath("exact_text_short.txt");
  const auto patternPath = makeFilePath("exact_pattern_longer.txt");

  writeFile(textPath, "AC");
  writeFile(patternPath, "ACG");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               InputFormatError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenTextContainsInvalidDnaCharacters) {
  const auto textPath = makeFilePath("exact_invalid_text.txt");
  const auto patternPath = makeFilePath("exact_pattern_for_invalid_text.txt");

  writeFile(textPath, "ACBX");
  writeFile(patternPath, "AC");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               ValidationError);
}

TEST_F(ExactSearchServiceTest, ThrowsWhenPatternContainsInvalidDnaCharacters) {
  const auto textPath = makeFilePath("exact_text_for_invalid_pattern.txt");
  const auto patternPath = makeFilePath("exact_invalid_pattern.txt");

  writeFile(textPath, "ACGT");
  writeFile(patternPath, "AX");

  EXPECT_THROW(service.run(textPath.string(), patternPath.string()),
               ValidationError);
}