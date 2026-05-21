#pragma once

class IEpsProvider {
public:
  virtual ~IEpsProvider() = default;

  virtual double epsForKmerLength(int kmerLength) const = 0;
};