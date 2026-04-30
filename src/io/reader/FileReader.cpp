#include "FileReader.hpp"
#include "../../core/common/errors/Errors.hpp"
#include <fstream>

std::string FileReader::readLine(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw FileReadError("Could not open file: " + filePath);
  }
  std::string line;
  std::getline(file, line);
  return line;
}