#pragma once

#include <stdexcept>
#include <string>

class MlPipelineError : public std::runtime_error {
public:
  explicit MlPipelineError(const std::string &message)
      : std::runtime_error(message) {}
};

class FileReadError : public MlPipelineError {
public:
  explicit FileReadError(const std::string &message)
      : MlPipelineError(message) {}
};

class InputFormatError : public MlPipelineError {
public:
  explicit InputFormatError(const std::string &message)
      : MlPipelineError(message) {}
};

class ValidationError : public MlPipelineError {
public:
  explicit ValidationError(const std::string &message)
      : MlPipelineError(message) {}
};

class ClusteringError : public MlPipelineError {
public:
  explicit ClusteringError(const std::string &message)
      : MlPipelineError(message) {}
};

class ReportWriteError : public MlPipelineError {
public:
  explicit ReportWriteError(const std::string &message)
      : MlPipelineError(message) {}
};