#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class RabinKarpSearcher {
public:
  std::vector<std::size_t> find(const std::string &text,
                                const std::string &pattern) const;

private:
  static const std::uint64_t _p = 257ULL;
  static const std::uint64_t _m = 1000000007ULL;

  std::uint64_t calculateHash(const std::string &s) const;
  std::uint64_t recalculateHash(std::uint64_t oldHash, char oldChar,
                                char newChar, std::uint64_t highestPower) const;
  bool isMatch(const std::string &text, std::size_t start,
               const std::string &pattern) const;

  std::uint64_t calculateHighestPower(std::size_t length) const;
};