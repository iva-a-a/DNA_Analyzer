#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Vocabulary {
public:
  void fit(const std::vector<std::vector<std::string>> &documents);

  bool contains(const std::string &token) const;
  std::size_t indexOf(const std::string &token) const;
  const std::vector<std::string> &tokens() const;
  std::size_t size() const;

private:
  std::vector<std::string> _tokens;
  std::unordered_map<std::string, std::size_t> _tokenToIndex;

  void clear();
};