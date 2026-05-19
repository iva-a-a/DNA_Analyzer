#include "DatasetLoader.hpp"

#include <fstream>

#include "../../common/Errors.hpp"
#include "../parser/DatasetParser.hpp"
#include "../validator/DatasetValidator.hpp"

namespace DatasetLoader {

DnaDataset load(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw FileReadError("Failed to open file: " + filename);
  }

  DnaDataset dataset = DatasetParser::parse(file);

  DatasetValidator::validate(dataset);

  return dataset;
}

} // namespace DatasetLoader