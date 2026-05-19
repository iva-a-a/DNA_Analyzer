#pragma once

#include "../data/DnaDataset.hpp"

namespace DatasetLoader {

DnaDataset load(const std::string &filename);

} // namespace DatasetLoader