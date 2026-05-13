#pragma once

#include <string>
#include <vector>

namespace FileReader {
std::string readLine(const std::string &filePath);
std::vector<std::string> readLines(const std::string &filePath);
} // namespace FileReader