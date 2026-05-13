#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../app/regexService/RegexService.hpp"
#include "../../core/common/errors/Errors.hpp"

namespace {

class RegexServiceTest : public ::testing::Test {
protected:
  RegexService service;
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

TEST_F(RegexServiceTest, ReturnsTrueForExactMatch) {
  const auto path = makeFilePath("regex_exact_true.txt");

  writeFile(path, "ACGT\nACGT");

  EXPECT_TRUE(service.run(path.string()));
}

TEST_F(RegexServiceTest, ReturnsFalseForExactMismatch) {
  const auto path = makeFilePath("regex_exact_false.txt");

  writeFile(path, "ACGT\nACGA");

  EXPECT_FALSE(service.run(path.string()));
}

TEST_F(RegexServiceTest, SupportsDotWildcard) {
  const auto path = makeFilePath("regex_dot_wildcard.txt");

  writeFile(path, "ACGT\nA.GT");

  EXPECT_TRUE(service.run(path.string()));
}

TEST_F(RegexServiceTest, SupportsStarWildcard) {
  const auto path = makeFilePath("regex_star_wildcard.txt");

  writeFile(path, "AA\nA*");

  EXPECT_TRUE(service.run(path.string()));
}

TEST_F(RegexServiceTest, SupportsDotStarWildcard) {
  const auto path = makeFilePath("regex_dot_star.txt");

  writeFile(path, "ACGT\nA.*T");

  EXPECT_TRUE(service.run(path.string()));
}

TEST_F(RegexServiceTest, ReturnsFalseWhenPatternDoesNotMatchEntireSequence) {
  const auto path = makeFilePath("regex_partial_not_enough.txt");

  writeFile(path, "ACGT\nAC");

  EXPECT_FALSE(service.run(path.string()));
}

TEST_F(RegexServiceTest, HandlesEmptySequenceAndEmptyPattern) {
  const auto path = makeFilePath("regex_empty_sequence_empty_pattern.txt");

  writeFile(path, "\n\n");

  EXPECT_THROW(service.run(path.string()), ValidationError);
}

TEST_F(RegexServiceTest, EmptyPatternDoesNotMatchNonEmptySequence) {
  const auto path = makeFilePath("regex_empty_pattern_non_empty_sequence.txt");

  writeFile(path, "A\n");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(RegexServiceTest, ThrowsWhenFileDoesNotExist) {
  const auto path = makeFilePath("regex_missing.txt");

  std::error_code ec;
  std::filesystem::remove(path, ec);

  EXPECT_THROW(service.run(path.string()), FileReadError);
}

TEST_F(RegexServiceTest, ThrowsWhenInputHasNoLines) {
  const auto path = makeFilePath("regex_no_lines.txt");

  writeFile(path, "");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(RegexServiceTest, ThrowsWhenInputHasOneLine) {
  const auto path = makeFilePath("regex_one_line.txt");

  writeFile(path, "ACGT");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(RegexServiceTest, ThrowsWhenInputHasTooManyLines) {
  const auto path = makeFilePath("regex_too_many_lines.txt");

  writeFile(path, "ACGT\nA.*\nextra");

  EXPECT_THROW(service.run(path.string()), InputFormatError);
}

TEST_F(RegexServiceTest, ThrowsWhenSequenceContainsInvalidDnaCharacters) {
  const auto path = makeFilePath("regex_invalid_sequence.txt");

  writeFile(path, "ACBX\nA.*");

  EXPECT_THROW(service.run(path.string()), ValidationError);
}

TEST_F(RegexServiceTest, DoesNotValidatePatternAsDnaSequence) {
  const auto path = makeFilePath("regex_pattern_not_dna.txt");

  writeFile(path, "ACGT\nA.*T");

  EXPECT_TRUE(service.run(path.string()));
}