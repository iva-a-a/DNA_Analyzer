#include "DnaSequence.hpp"
#include "../errors/Errors.hpp"
#include "../validation/Validation.hpp"

DnaSequence::DnaSequence(const std::string &sequence) {
  if (!Validation::IsDnaString(sequence)) {
    throw ValidationError("DnaSequence contains invalid characters");
  }
  _sequence = sequence;
}

std::size_t DnaSequence::size() const noexcept { return _sequence.size(); }

bool DnaSequence::empty() const noexcept { return _sequence.empty(); }

const std::string &DnaSequence::getSequence() const noexcept { return _sequence; }