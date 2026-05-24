#pragma once

#include <string>

#include "ClusteringReport.hpp"

class MarkdownReportWriter {
public:
  void write(const std::string &outputPath,
             const ClusteringReport &report) const;

private:
  void header(std::ofstream &outFile) const;
  void resultTable(std::ofstream &outFile,
                   const ClusteringReport &report) const;
  void notes(std::ofstream &outFile) const;
  void conclusions(std::ofstream &outFile,
                   const ClusteringReport &report) const;
};