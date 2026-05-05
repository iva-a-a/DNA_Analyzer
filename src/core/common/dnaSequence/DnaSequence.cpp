#include "DnaSequence.hpp"
#include "../errors/Errors.hpp"
#include "../validation/Validation.hpp"

DnaSequence::DnaSequence() : _sequence("") {}

DnaSequence::DnaSequence(const std::string &sequence) {
  if (!Validation::isDnaString(sequence)) {
    throw ValidationError("DnaSequence contains invalid characters");
  }
  _sequence = sequence;
}

std::size_t DnaSequence::size() const noexcept { return _sequence.size(); }

bool DnaSequence::empty() const noexcept { return _sequence.empty(); }

const std::string &DnaSequence::value() const noexcept { return _sequence; }