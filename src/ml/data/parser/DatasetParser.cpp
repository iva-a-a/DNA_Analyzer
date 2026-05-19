#include "DatasetParser.hpp"

#include <sstream>

#include "../../common/Errors.hpp"

namespace {

std::vector<std::string> splitCsvLine(const std::string &line) {
  std::vector<std::string> columns;
  std::stringstream ss(line);
  std::string column;

  while (std::getline(ss, column, ',')) {
    columns.push_back(column);
  }

  return columns;
}

DnaRecord parseRecord(const std::vector<std::string> &columns,
                      std::size_t lineNumber) {
  if (columns.size() != 3) {
    throw InputFormatError("Invalid CSV format at line " +
                           std::to_string(lineNumber) +
                           ": expected 3 columns: sequence,classId,species");
  }

  DnaRecord record;
  record.sequence = columns[0];

  try {
    record.classId = std::stoi(columns[1]);
  } catch (const std::exception &) {
    throw InputFormatError("Invalid classId at line " +
                           std::to_string(lineNumber) +
                           ": value must be integer");
  }

  record.species = columns[2];

  return record;
}

} // namespace

namespace DatasetParser {

DnaDataset parse(std::istream &input) {
  std::vector<DnaRecord> records;
  std::string line;
  std::size_t lineNumber = 0;

  if (std::getline(input, line)) {
    ++lineNumber;
  }

  while (std::getline(input, line)) {
    ++lineNumber;

    if (line.empty()) {
      continue;
    }

    std::vector<std::string> columns = splitCsvLine(line);
    records.push_back(parseRecord(columns, lineNumber));
  }

  return DnaDataset(std::move(records));
}

} // namespace DatasetParser