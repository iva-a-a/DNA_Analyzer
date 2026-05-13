#pragma once

#include <stdexcept>
#include <string>

class DnaAnalyzerError : public std::runtime_error {
public:
  explicit DnaAnalyzerError(const std::string &message)
      : std::runtime_error(message) {}
};

class InputFormatError : public DnaAnalyzerError {
public:
  explicit InputFormatError(const std::string &message)
      : DnaAnalyzerError(message) {}
};

class ValidationError : public DnaAnalyzerError {
public:
  explicit ValidationError(const std::string &message)
      : DnaAnalyzerError(message) {}
};

class FileReadError : public DnaAnalyzerError {
public:
  explicit FileReadError(const std::string &message)
      : DnaAnalyzerError(message) {}
};