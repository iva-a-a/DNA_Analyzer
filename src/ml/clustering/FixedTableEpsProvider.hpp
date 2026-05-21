#pragma once

#include "IEpsProvider.hpp"

class FixedTableEpsProvider : public IEpsProvider {
public:
  double epsForKmerLength(int kmerLength) const override;
};