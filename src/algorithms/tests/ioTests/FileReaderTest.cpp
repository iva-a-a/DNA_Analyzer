#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../core/common/errors/Errors.hpp"
#include "../../io/reader/FileReader.hpp"

namespace {

std::filesystem::path makeTempFilePath(const std::string &fileName) {
  return std::filesystem::temp_directory_path() / fileName;
}

void writeFile(const std::filesystem::path &path, const std::string &content) {
  std::ofstream file(path);
  ASSERT_TRUE(file.is_open());
  file << content;
}

void removeFileIfExists(const std::filesystem::path &path) {
  std::error_code ec;
  std::filesystem::remove(path, ec);
}

} // namespace

TEST(FileReaderReadLineTest, ReadsSingleLineFile) {
  const auto path = makeTempFilePath("file_reader_single_line.txt");
  writeFile(path, "hello");

  EXPECT_EQ(FileReader::readLine(path.string()), "hello");

  removeFileIfExists(path);
}

TEST(FileReaderReadLineTest, ReadsOnlyFirstLineFromMultiLineFile) {
  const auto path = makeTempFilePath("file_reader_multi_line.txt");
  writeFile(path, "first\nsecond\nthird");

  EXPECT_EQ(FileReader::readLine(path.string()), "first");

  removeFileIfExists(path);
}

TEST(FileReaderReadLineTest, EmptyFileReturnsEmptyString) {
  const auto path = makeTempFilePath("file_reader_empty.txt");
  writeFile(path, "");

  EXPECT_EQ(FileReader::readLine(path.string()), "");

  removeFileIfExists(path);
}

TEST(FileReaderReadLineTest, FirstLineCanBeEmpty) {
  const auto path = makeTempFilePath("file_reader_first_line_empty.txt");
  writeFile(path, "\nsecond");

  EXPECT_EQ(FileReader::readLine(path.string()), "");

  removeFileIfExists(path);
}

TEST(FileReaderReadLineTest, PreservesSpacesInLine) {
  const auto path = makeTempFilePath("file_reader_spaces.txt");
  writeFile(path, "  hello world  \nnext");

  EXPECT_EQ(FileReader::readLine(path.string()), "  hello world  ");

  removeFileIfExists(path);
}

TEST(FileReaderReadLineTest, ThrowsWhenFileDoesNotExist) {
  const auto path = makeTempFilePath("file_reader_missing.txt");
  removeFileIfExists(path);

  EXPECT_THROW(FileReader::readLine(path.string()), FileReadError);
}

TEST(FileReaderReadLinesTest, ReadsAllLines) {
  const auto path = makeTempFilePath("file_reader_lines.txt");
  writeFile(path, "first\nsecond\nthird");

  const std::vector<std::string> expected = {"first", "second", "third"};

  EXPECT_EQ(FileReader::readLines(path.string()), expected);

  removeFileIfExists(path);
}

TEST(FileReaderReadLinesTest, EmptyFileReturnsEmptyVector) {
  const auto path = makeTempFilePath("file_reader_empty_lines.txt");
  writeFile(path, "");

  EXPECT_TRUE(FileReader::readLines(path.string()).empty());

  removeFileIfExists(path);
}

TEST(FileReaderReadLinesTest, PreservesEmptyLines) {
  const auto path = makeTempFilePath("file_reader_empty_lines_inside.txt");
  writeFile(path, "first\n\nthird");

  const std::vector<std::string> expected = {"first", "", "third"};

  EXPECT_EQ(FileReader::readLines(path.string()), expected);

  removeFileIfExists(path);
}

TEST(FileReaderReadLinesTest, PreservesLeadingAndTrailingSpaces) {
  const auto path = makeTempFilePath("file_reader_line_spaces.txt");
  writeFile(path, "  first  \n second\nthird ");

  const std::vector<std::string> expected = {
      "  first  ",
      " second",
      "third ",
  };

  EXPECT_EQ(FileReader::readLines(path.string()), expected);

  removeFileIfExists(path);
}

TEST(FileReaderReadLinesTest, HandlesTrailingNewlineWithoutExtraEmptyLine) {
  const auto path = makeTempFilePath("file_reader_trailing_newline.txt");
  writeFile(path, "first\nsecond\n");

  const std::vector<std::string> expected = {"first", "second"};

  EXPECT_EQ(FileReader::readLines(path.string()), expected);

  removeFileIfExists(path);
}

TEST(FileReaderReadLinesTest, SingleNewlineFileReturnsOneEmptyLine) {
  const auto path = makeTempFilePath("file_reader_single_newline.txt");
  writeFile(path, "\n");

  const std::vector<std::string> expected = {""};

  EXPECT_EQ(FileReader::readLines(path.string()), expected);

  removeFileIfExists(path);
}

TEST(FileReaderReadLinesTest, ThrowsWhenFileDoesNotExist) {
  const auto path = makeTempFilePath("file_reader_missing_lines.txt");
  removeFileIfExists(path);

  EXPECT_THROW(FileReader::readLines(path.string()), FileReadError);
}