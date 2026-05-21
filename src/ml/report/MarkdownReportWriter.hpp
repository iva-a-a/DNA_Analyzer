#pragma once

#include <string>

#include "ClusteringReport.hpp"

class MarkdownReportWriter {
public:
  void write(const std::string &outputPath,
             const ClusteringReport &report) const;
};