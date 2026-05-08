#include "FileReader.hpp"

#include <fstream>

#include "../../core/common/errors/Errors.hpp"

template <typename Func>
auto FileReader::withFile(const std::string &filePath, Func func) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw FileReadError("Could not open file: " + filePath);
  }
  return func(file);
}

std::string FileReader::readLine(const std::string &filePath) {
  return withFile(filePath, [](std::ifstream &file) {
    std::string line;
    std::getline(file, line);
    return line;
  });
}

std::vector<std::string> FileReader::readLines(const std::string &filePath) {
  return withFile(filePath, [](std::ifstream &file) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
      lines.push_back(line);
    }
    return lines;
  });
}