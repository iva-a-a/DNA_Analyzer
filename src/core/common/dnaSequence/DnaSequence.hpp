#pragma once

#include <cstddef>
#include <string>

class DnaSequence {
public:
  DnaSequence() = default;
  explicit DnaSequence(const std::string &sequence);

  std::size_t size() const noexcept;
  bool empty() const noexcept;

  const std::string &getSequence() const noexcept;

private:
  std::string _sequence;
};