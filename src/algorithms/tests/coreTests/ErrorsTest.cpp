#include <gtest/gtest.h>

#include "../../core/common/errors/Errors.hpp"

TEST(ErrorsTest, DnaAnalyzerErrorStoresMessage) {
  DnaAnalyzerError error("base error");

  EXPECT_STREQ(error.what(), "base error");
}

TEST(ErrorsTest, InputFormatErrorStoresMessage) {
  InputFormatError error("input format error");

  EXPECT_STREQ(error.what(), "input format error");
}

TEST(ErrorsTest, ValidationErrorStoresMessage) {
  ValidationError error("validation error");

  EXPECT_STREQ(error.what(), "validation error");
}

TEST(ErrorsTest, FileReadErrorStoresMessage) {
  FileReadError error("file read error");

  EXPECT_STREQ(error.what(), "file read error");
}

TEST(ErrorsTest, InputFormatErrorCanBeCaughtAsDnaAnalyzerError) {
  try {
    throw InputFormatError("input format error");
  } catch (const DnaAnalyzerError &error) {
    EXPECT_STREQ(error.what(), "input format error");
  }
}

TEST(ErrorsTest, ValidationErrorCanBeCaughtAsDnaAnalyzerError) {
  try {
    throw ValidationError("validation error");
  } catch (const DnaAnalyzerError &error) {
    EXPECT_STREQ(error.what(), "validation error");
  }
}

TEST(ErrorsTest, FileReadErrorCanBeCaughtAsDnaAnalyzerError) {
  try {
    throw FileReadError("file read error");
  } catch (const DnaAnalyzerError &error) {
    EXPECT_STREQ(error.what(), "file read error");
  }
}

TEST(ErrorsTest, DnaAnalyzerErrorCanBeCaughtAsRuntimeError) {
  try {
    throw DnaAnalyzerError("runtime error");
  } catch (const std::runtime_error &error) {
    EXPECT_STREQ(error.what(), "runtime error");
  }
}