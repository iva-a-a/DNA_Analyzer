#include "Vocabulary.hpp"

#include "../common/Errors.hpp"

void Vocabulary::fit(const std::vector<std::vector<std::string>> &documents) {
  clear();
  for (const auto &document : documents) {
    for (const auto &token : document) {
      if (!contains(token)) {
        _tokens.push_back(token);
        _tokenToIndex[token] = _tokens.size() - 1;
      }
    }
  }
}

bool Vocabulary::contains(const std::string &token) const {
  return _tokenToIndex.find(token) != _tokenToIndex.end();
}
std::size_t Vocabulary::indexOf(const std::string &token) const {
  auto it = _tokenToIndex.find(token);
  if (it == _tokenToIndex.end()) {
    throw ValidationError("Token not found in vocabulary: " + token);
  }
  return it->second;
}
const std::vector<std::string> &Vocabulary::tokens() const { return _tokens; }
std::size_t Vocabulary::size() const { return _tokens.size(); }

void Vocabulary::clear() {
  _tokens.clear();
  _tokenToIndex.clear();
}