#include <gtest/gtest.h>

#include "../../common/Errors.hpp"

TEST(MlErrorsTest, MlPipelineErrorStoresMessage) {
  MlPipelineError error("base error");

  EXPECT_STREQ(error.what(), "base error");
}

TEST(MlErrorsTest, FileReadErrorStoresMessage) {
  FileReadError error("file read error");

  EXPECT_STREQ(error.what(), "file read error");
}

TEST(MlErrorsTest, InputFormatErrorStoresMessage) {
  InputFormatError error("input format error");

  EXPECT_STREQ(error.what(), "input format error");
}

TEST(MlErrorsTest, ValidationErrorStoresMessage) {
  ValidationError error("validation error");

  EXPECT_STREQ(error.what(), "validation error");
}

TEST(MlErrorsTest, ClusteringErrorStoresMessage) {
  ClusteringError error("clustering error");

  EXPECT_STREQ(error.what(), "clustering error");
}

TEST(MlErrorsTest, ReportWriteErrorStoresMessage) {
  ReportWriteError error("report write error");

  EXPECT_STREQ(error.what(), "report write error");
}

TEST(MlErrorsTest, FileReadErrorCanBeCaughtAsMlPipelineError) {
  try {
    throw FileReadError("file read error");
  } catch (const MlPipelineError &error) {
    EXPECT_STREQ(error.what(), "file read error");
  }
}

TEST(MlErrorsTest, InputFormatErrorCanBeCaughtAsMlPipelineError) {
  try {
    throw InputFormatError("input format error");
  } catch (const MlPipelineError &error) {
    EXPECT_STREQ(error.what(), "input format error");
  }
}

TEST(MlErrorsTest, ValidationErrorCanBeCaughtAsMlPipelineError) {
  try {
    throw ValidationError("validation error");
  } catch (const MlPipelineError &error) {
    EXPECT_STREQ(error.what(), "validation error");
  }
}

TEST(MlErrorsTest, ClusteringErrorCanBeCaughtAsMlPipelineError) {
  try {
    throw ClusteringError("clustering error");
  } catch (const MlPipelineError &error) {
    EXPECT_STREQ(error.what(), "clustering error");
  }
}

TEST(MlErrorsTest, ReportWriteErrorCanBeCaughtAsMlPipelineError) {
  try {
    throw ReportWriteError("report write error");
  } catch (const MlPipelineError &error) {
    EXPECT_STREQ(error.what(), "report write error");
  }
}

TEST(MlErrorsTest, MlPipelineErrorCanBeCaughtAsRuntimeError) {
  try {
    throw MlPipelineError("runtime error");
  } catch (const std::runtime_error &error) {
    EXPECT_STREQ(error.what(), "runtime error");
  }
}
