#pragma once

#include "../CommandLineOptions.hpp"

class CommandDispatcher {
public:
  int dispatch(const CommandLineOptions &options) const;

private:
  int runExactSearch(const CommandLineOptions &options) const;
  int runAlignScore(const CommandLineOptions &options) const;
  int runAlign(const CommandLineOptions &options) const;
  int runRegexMatch(const CommandLineOptions &options) const;
  int runKSimilarity(const CommandLineOptions &options) const;
  int runMinWindow(const CommandLineOptions &options) const;
};