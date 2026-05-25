#include "FileReader.hpp"

#include <fstream>

#include "../../core/common/errors/Errors.hpp"

namespace {

template <typename Func> auto withFile(const std::string &filePath, Func func) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw FileReadError("Could not open file: " + filePath);
  }

  return func(file);
}

} // namespace

namespace FileReader {

std::vector<std::string> readLines(const std::string &filePath) {
  return withFile(filePath, [](std::ifstream &file) {
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
      lines.push_back(line);
    }

    return lines;
  });
}

} // namespace FileReader