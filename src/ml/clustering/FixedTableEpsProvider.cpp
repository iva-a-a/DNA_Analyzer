#include "FixedTableEpsProvider.hpp"

double FixedTableEpsProvider::epsForKmerLength(int kmerLength) const {
  switch (kmerLength) {
  case 1:
    return 0.028;
  case 2:
    return 0.064;
  case 3:
    return 0.090;
  case 4:
    return 0.101;
  case 5:
    return 0.105;
  case 6:
    return 0.107;
  default:
    return 0.090;
  }
}