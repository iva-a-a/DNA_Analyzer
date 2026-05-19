#include "DnaDataset.hpp"

#include <set>

DnaDataset::DnaDataset(std::vector<DnaRecord> records)
    : _records(std::move(records)) {}

const std::vector<DnaRecord> &DnaDataset::records() const { return _records; }

std::size_t DnaDataset::size() const { return _records.size(); }

bool DnaDataset::empty() const { return _records.empty(); }

std::vector<std::string> DnaDataset::sequences() const {
  std::vector<std::string> seqs;
  for (const auto &record : _records) {
    seqs.push_back(record.sequence);
  }
  return seqs;
}

std::vector<int> DnaDataset::classIds() const {
  std::vector<int> ids;
  for (const auto &record : _records) {
    ids.push_back(record.classId);
  }
  return ids;
}

std::vector<std::string> DnaDataset::species() const {
  std::vector<std::string> spcs;
  for (const auto &record : _records) {
    spcs.push_back(record.species);
  }
  return spcs;
}

int DnaDataset::classCount() const {
  std::set<int> uniqueClasses =
      std::set<int>(classIds().begin(), classIds().end());
  return uniqueClasses.size();
}