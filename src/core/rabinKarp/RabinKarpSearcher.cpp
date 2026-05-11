#include "RabinKarpSearcher.hpp"

inline static const std::uint64_t BASE = 257ULL;
inline static const std::uint64_t MOD = 1000000007ULL;

std::vector<std::size_t>
RabinKarpSearcher::find(const std::string &text,
                        const std::string &pattern) const {
  std::vector<std::size_t> positions;
  std::size_t patternLength = pattern.size();
  std::size_t textLength = text.size();

  if (patternLength == 0 || textLength < patternLength) {
    return positions;
  }

  std::uint64_t patternHash = calculateHash(pattern);
  std::uint64_t windowHash = calculateHash(text.substr(0, patternLength));
  std::uint64_t highestPower = calculateHighestPower(patternLength);

  for (std::size_t left = 0; left <= textLength - patternLength; ++left) {
    if (windowHash == patternHash && isMatch(text, left, pattern)) {
      positions.push_back(left);
    }

    if (left < textLength - patternLength) {
      windowHash = recalculateHash(windowHash, text[left],
                                   text[left + patternLength], highestPower);
    }
  }
  return positions;
}

std::uint64_t RabinKarpSearcher::calculateHash(const std::string &text) const {
  std::uint64_t hashValue = 0;

  for (char c : text) {
    auto value = static_cast<unsigned char>(c) + 1;
    hashValue = (hashValue * BASE + value) % MOD;
  }

  return hashValue;
}

std::uint64_t
RabinKarpSearcher::recalculateHash(std::uint64_t oldHash, char oldChar,
                                   char newChar,
                                   std::uint64_t highestPower) const {
  auto oldValue = static_cast<unsigned char>(oldChar) + 1;
  auto newValue = static_cast<unsigned char>(newChar) + 1;

  std::uint64_t removed = (oldValue * highestPower) % MOD;
  std::uint64_t newHash = (oldHash + MOD - removed) % MOD;

  newHash = (newHash * BASE) % MOD;
  newHash = (newHash + newValue) % MOD;

  return newHash;
}

bool RabinKarpSearcher::isMatch(const std::string &text, std::size_t start,
                                const std::string &pattern) const {
  if (start + pattern.size() > text.size()) {
    return false;
  }

  for (std::size_t i = 0; i < pattern.size(); ++i) {
    if (text[start + i] != pattern[i]) {
      return false;
    }
  }

  return true;
}

std::uint64_t
RabinKarpSearcher::calculateHighestPower(std::size_t length) const {
  std::uint64_t power = 1;
  for (std::size_t i = 0; i < length - 1; ++i) {
    power = (power * BASE) % MOD;
  }
  return power;
}
