#include <iostream>
#include <string>
#include <vector>

namespace FileReader {
template <typename Func> auto withFile(const std::string &filePath, Func func);
std::string readLine(const std::string &filePath);
std::vector<std::string> readLines(const std::string &filePath);
} // namespace FileReader