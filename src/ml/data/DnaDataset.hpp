#pragma once

#include <vector>

#include "DnaRecord.hpp"

class DnaDataset {
public:
  explicit DnaDataset(std::vector<DnaRecord> records);

  const std::vector<DnaRecord> &records() const;
  std::size_t size() const;
  bool empty() const;
  std::vector<std::string> sequences() const;
  std::vector<int> classIds() const;
  std::vector<std::string> species() const;
  int classCount() const;

private:
  std::vector<DnaRecord> _records;
};