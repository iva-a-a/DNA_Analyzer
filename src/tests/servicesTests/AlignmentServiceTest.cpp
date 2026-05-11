#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../app/alignmentService/AlignmentService.hpp"
#include "../../core/common/errors/Errors.hpp"

namespace {

class AlignmentServiceTest : public ::testing::Test {
protected:
  AlignmentService service;
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

TEST_F(AlignmentServiceTest, RunScoreOnlyIdenticalSequences) {
  const auto path = makeFilePath("alignment_score_identical.txt");

  writeFile(path, "1 -1 -2\nACGT\nACGT");

  EXPECT_EQ(service.runScoreOnly(path.string()), 4);
}

TEST_F(AlignmentServiceTest, RunFullAlignmentIdenticalSequences) {
  const auto path = makeFilePath("alignment_full_identical.txt");

  writeFile(path, "1 -1 -2\nACGT\nACGT");

  const AlignmentResult result = service.runFullAlignment(path.string());

  EXPECT_EQ(result.score, 4);
  EXPECT_EQ(result.alignedFirst, "ACGT");
  EXPECT_EQ(result.matchLine, "||||");
  EXPECT_EQ(result.alignedSecond, "ACGT");
}

TEST_F(AlignmentServiceTest, RunScoreOnlyAllMismatches) {
  const auto path = makeFilePath("alignment_score_mismatches.txt");

  writeFile(path, "1 -1 -2\nAAAA\nTTTT");

  EXPECT_EQ(service.runScoreOnly(path.string()), -4);
}

TEST_F(AlignmentServiceTest, RunFullAlignmentAllMismatches) {
  const auto path = makeFilePath("alignment_full_mismatches.txt");

  writeFile(path, "1 -1 -2\nAAAA\nTTTT");

  const AlignmentResult result = service.runFullAlignment(path.string());

  EXPECT_EQ(result.score, -4);
  EXPECT_EQ(result.alignedFirst, "AAAA");
  EXPECT_EQ(result.matchLine, "    ");
  EXPECT_EQ(result.alignedSecond, "TTTT");
}

TEST_F(AlignmentServiceTest, RunScoreOnlyWithGap) {
  const auto path = makeFilePath("alignment_score_gap.txt");

  writeFile(path, "1 -1 -2\nACGT\nAGT");

  EXPECT_EQ(service.runScoreOnly(path.string()), 1);
}

TEST_F(AlignmentServiceTest, RunFullAlignmentWithGap) {
  const auto path = makeFilePath("alignment_full_gap.txt");

  writeFile(path, "1 -1 -2\nACGT\nAGT");

  const AlignmentResult result = service.runFullAlignment(path.string());

  EXPECT_EQ(result.score, 1);
  EXPECT_EQ(result.alignedFirst, "ACGT");
  EXPECT_EQ(result.matchLine, "| ||");
  EXPECT_EQ(result.alignedSecond, "A-GT");
}

TEST_F(AlignmentServiceTest, RunScoreOnlyFirstSequenceEmpty) {
  const auto path = makeFilePath("alignment_score_first_empty.txt");

  writeFile(path, "1 -1 -2\n\nACG");

  EXPECT_EQ(service.runScoreOnly(path.string()), -6);
}

TEST_F(AlignmentServiceTest, RunFullAlignmentFirstSequenceEmpty) {
  const auto path = makeFilePath("alignment_full_first_empty.txt");

  writeFile(path, "1 -1 -2\n\nACG");

  const AlignmentResult result = service.runFullAlignment(path.string());

  EXPECT_EQ(result.score, -6);
  EXPECT_EQ(result.alignedFirst, "---");
  EXPECT_EQ(result.matchLine, "   ");
  EXPECT_EQ(result.alignedSecond, "ACG");
}

TEST_F(AlignmentServiceTest, ThrowsWhenSecondSequenceMissing) {
  const auto path = makeFilePath("alignment_second_missing.txt");

  writeFile(path, "1 -1 -2\nACG\n");

  EXPECT_THROW(service.runScoreOnly(path.string()), InputFormatError);
  EXPECT_THROW(service.runFullAlignment(path.string()), InputFormatError);
}

TEST_F(AlignmentServiceTest, ThrowsWhenBothSequencesMissing) {
  const auto path = makeFilePath("alignment_both_missing.txt");

  writeFile(path, "1 -1 -2\n");

  EXPECT_THROW(service.runScoreOnly(path.string()), InputFormatError);
  EXPECT_THROW(service.runFullAlignment(path.string()), InputFormatError);
}

TEST_F(AlignmentServiceTest, SupportsCustomScoringScheme) {
  const auto path = makeFilePath("alignment_custom_scoring.txt");

  writeFile(path, "2 -3 -4\nAC\nAC");

  EXPECT_EQ(service.runScoreOnly(path.string()), 4);

  const AlignmentResult result = service.runFullAlignment(path.string());

  EXPECT_EQ(result.score, 4);
  EXPECT_EQ(result.alignedFirst, "AC");
  EXPECT_EQ(result.matchLine, "||");
  EXPECT_EQ(result.alignedSecond, "AC");
}

TEST_F(AlignmentServiceTest, PrefersMismatchWhenGapIsWorse) {
  const auto path = makeFilePath("alignment_prefers_mismatch.txt");

  writeFile(path, "1 -1 -5\nA\nT");

  const AlignmentResult result = service.runFullAlignment(path.string());

  EXPECT_EQ(result.score, -1);
  EXPECT_EQ(result.alignedFirst, "A");
  EXPECT_EQ(result.matchLine, " ");
  EXPECT_EQ(result.alignedSecond, "T");
}

TEST_F(AlignmentServiceTest, HandlesLongerValidDnaSequences) {
  const auto path = makeFilePath("alignment_long_sequences.txt");

  writeFile(path, "1 -1 -2\nACGTACGT\nACGTTGCA");

  EXPECT_EQ(service.runScoreOnly(path.string()), 0);
}

TEST_F(AlignmentServiceTest, ThrowsWhenFileDoesNotExist) {
  const auto path = makeFilePath("alignment_missing_file.txt");

  std::error_code ec;
  std::filesystem::remove(path, ec);

  EXPECT_THROW(service.runScoreOnly(path.string()), FileReadError);
  EXPECT_THROW(service.runFullAlignment(path.string()), FileReadError);
}

TEST_F(AlignmentServiceTest, ThrowsWhenInputHasTooFewLines) {
  const auto path = makeFilePath("alignment_too_few_lines.txt");

  writeFile(path, "1 -1 -2\nACGT");

  EXPECT_THROW(service.runScoreOnly(path.string()), InputFormatError);
  EXPECT_THROW(service.runFullAlignment(path.string()), InputFormatError);
}

TEST_F(AlignmentServiceTest, ThrowsWhenInputHasTooManyLines) {
  const auto path = makeFilePath("alignment_too_many_lines.txt");

  writeFile(path, "1 -1 -2\nACGT\nACGT\nextra");

  EXPECT_THROW(service.runScoreOnly(path.string()), InputFormatError);
  EXPECT_THROW(service.runFullAlignment(path.string()), InputFormatError);
}

TEST_F(AlignmentServiceTest, ThrowsWhenScoringLineIsInvalid) {
  const auto path = makeFilePath("alignment_invalid_scoring.txt");

  writeFile(path, "1 x -2\nACGT\nACGT");

  EXPECT_THROW(service.runScoreOnly(path.string()), InputFormatError);
  EXPECT_THROW(service.runFullAlignment(path.string()), InputFormatError);
}

TEST_F(AlignmentServiceTest, ThrowsWhenScoringLineHasTooFewIntegers) {
  const auto path = makeFilePath("alignment_too_few_scores.txt");

  writeFile(path, "1 -1\nACGT\nACGT");

  EXPECT_THROW(service.runScoreOnly(path.string()), InputFormatError);
  EXPECT_THROW(service.runFullAlignment(path.string()), InputFormatError);
}

TEST_F(AlignmentServiceTest, ThrowsWhenFirstSequenceHasInvalidDnaCharacters) {
  const auto path = makeFilePath("alignment_invalid_first_dna.txt");

  writeFile(path, "1 -1 -2\nACBX\nACGT");

  EXPECT_THROW(service.runScoreOnly(path.string()), ValidationError);
  EXPECT_THROW(service.runFullAlignment(path.string()), ValidationError);
}

TEST_F(AlignmentServiceTest, ThrowsWhenSecondSequenceHasInvalidDnaCharacters) {
  const auto path = makeFilePath("alignment_invalid_second_dna.txt");

  writeFile(path, "1 -1 -2\nACGT\nACBX");

  EXPECT_THROW(service.runScoreOnly(path.string()), ValidationError);
  EXPECT_THROW(service.runFullAlignment(path.string()), ValidationError);
}