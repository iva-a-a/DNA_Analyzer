#include "Vectorizer.hpp"

#include "../common/Errors.hpp"
#include "KmerExtractor.hpp"
#include "Vocabulary.hpp"

std::vector<std::vector<double>>
Vectorizer::fitTransform(const std::vector<std::string> &sequences,
                         std::size_t k) const {
  if (sequences.empty()) {
    throw ValidationError("Cannot vectorize empty sequence list");
  }

  std::vector<std::vector<std::string>> documents;

  for (const auto &sequence : sequences) {
    documents.push_back(KmerExtractor::extractKmers(sequence, k));
  }

  Vocabulary vocabulary;
  vocabulary.fit(documents);

  if (vocabulary.size() == 0) {
    throw ValidationError("Cannot build vocabulary: no k-mers found");
  }

  std::vector<std::vector<double>> vectors;

  for (const auto &document : documents) {
    std::vector<double> vector(vocabulary.size(), 0.0);

    for (const auto &token : document) {
      std::size_t index = vocabulary.indexOf(token);
      vector[index] += 1.0;
    }

    if (!document.empty()) {
      for (double &value : vector) {
        value /= static_cast<double>(document.size());
      }
    }

    vectors.push_back(std::move(vector));
  }

  return vectors;
}