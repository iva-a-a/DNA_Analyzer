#include "DatasetValidator.hpp"

#include "../../common/Errors.hpp"
#include "../../utils/DnaValidation.hpp"

namespace DatasetValidator {

void validate(const DnaDataset &dataset) {
  if (dataset.empty()) {
    throw ValidationError("Dataset is empty");
  }

  const std::vector<DnaRecord> &records = dataset.records();

  for (std::size_t i = 0; i < records.size(); ++i) {
    const DnaRecord &record = records[i];

    const std::string rowPrefix =
        "Invalid record at index " + std::to_string(i) + ": ";

    if (record.sequence.empty()) {
      throw ValidationError(rowPrefix + "DNA sequence is empty");
    }

    if (!DnaValidation::isDnaSequence(record.sequence)) {
      throw ValidationError(
          rowPrefix +
          "DNA sequence contains invalid characters: " + record.sequence);
    }

    if (record.classId < 0) {
      throw ValidationError(rowPrefix + "classId must be non-negative");
    }

    if (record.species.empty()) {
      throw ValidationError(rowPrefix + "species is empty");
    }
  }
}

} // namespace DatasetValidator