#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../app/kSimilarityService/KSimilarityService.hpp"
#include "../../core/common/errors/Errors.hpp"

namespace {

class KSimilarityServiceTest : public ::testing::Test {
protected:
  KSimilarityService service;
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

TEST_F(KSimilarityServiceTest, ReturnsZeroForEqualStrings) {
  const auto path = makeFilePath("k_similarity_equal.txt");

  writeFile(path, "ACGT\nACGT");

  EXPECT_EQ(service.run(path.string()), 0);
}

TEST_F(KSimilarityServiceTest, ReturnsOneForSingleSwap) {
  const auto path = makeFilePath("k_similarity_one_swap.txt");

  writeFile(path, "AC\nCA");

  EXPECT_EQ(service.run(path.string()), 1);
}

TEST_F(KSimilarityServiceTest, ReturnsTwoForTwoSwaps) {
  const auto path = makeFilePath("k_similarity_two_swaps.txt");

  writeFile(path, "ACGT\nTGCA");

  EXPECT_EQ(service.run(path.string()), 2);
}

TEST_F(KSimilarityServiceTest, HandlesRepeatedCharacters) {
  const auto path = makeFilePath("k_similarity_repeated.txt");

  writeFile(path, "AACC\nCCAA");

  EXPECT_EQ(service.run(path.string()), 2);
}

TEST_F(KSimilarityServiceTest, HandlesSingleCharacterStrings) {
  const auto path = makeFilePath("k_similarity_single_char.txt");

  writeFile(path, "A\nA");

  EXPECT_EQ(service.run(path.string()), 0);
}

TEST_F(KSimilarityServiceTest, ThrowsWhenFileDoesNotExist) {
  const auto path = makeFilePath("k_similarity_missing.txt");

  std::error_code ec;
  std::filesystem::remove(path, ec);

  EXPECT_THROW(service.run(path.string()), FileReadError);
}

TEST_F(KSimilarityServiceTest, ThrowsWhenInputHasNoLines) {
  const auto path = makeFilePath("k_similarity_no_lines.txt");

  writeFile(path, "");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(KSimilarityServiceTest, ThrowsWhenInputHasOneLine) {
  const auto path = makeFilePath("k_similarity_one_line.txt");

  writeFile(path, "ACGT");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(KSimilarityServiceTest, ThrowsWhenInputHasTooManyLines) {
  const auto path = makeFilePath("k_similarity_too_many_lines.txt");

  writeFile(path, "ACGT\nTGCA\nextra");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(KSimilarityServiceTest, AllowsEmptyStrings) {
  const auto path = makeFilePath("k_similarity_empty_strings.txt");

  writeFile(path, "\n\n");

  EXPECT_EQ(service.run(path.string()), 0);
}

TEST_F(KSimilarityServiceTest, ThrowsWhenSourceContainsInvalidDnaCharacters) {
  const auto path = makeFilePath("k_similarity_invalid_source.txt");

  writeFile(path, "ACBX\nACGT");

  EXPECT_THROW(service.run(path.string()), ValidationError);
}
TEST_F(KSimilarityServiceTest, ThrowsWhenStringsAreNotAnagrams) {
  const auto path = makeFilePath("k_similarity_not_anagrams.txt");

  writeFile(path, "ACGT\nTGCC");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}
TEST_F(KSimilarityServiceTest, ThrowsWhenTargetContainsInvalidDnaCharacters) {
  const auto path = makeFilePath("k_similarity_invalid_target.txt");

  writeFile(path, "ACGT\nACBX");

  EXPECT_THROW(service.run(path.string()), ValidationError);
}