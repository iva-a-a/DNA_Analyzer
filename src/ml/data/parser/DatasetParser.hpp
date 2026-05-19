#pragma once

#include <istream>

#include "../DnaDataset.hpp"

namespace DatasetParser {

DnaDataset parse(std::istream &input);

} // namespace DatasetParser