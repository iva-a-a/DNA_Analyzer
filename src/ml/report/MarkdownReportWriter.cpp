#include "MarkdownReportWriter.hpp"

#include <fstream>
#include <iomanip>

#include "../common/Errors.hpp"

void MarkdownReportWriter::write(const std::string &outputPath,
                                 const ClusteringReport &report) const {
  std::ofstream outFile(outputPath);

  if (!outFile.is_open()) {
    throw ReportWriteError("Failed to open output file: " + outputPath);
  }

  outFile << "# DNA Clustering Report\n\n";
  outFile << "## Rand Index Results\n\n";

  if (report.rows.empty()) {
    outFile << "No results available.\n";
    return;
  }

  outFile << "| K-mer |";
  for (const auto &score : report.rows.front().scores) {
    outFile << " " << score.algorithmName << " |";
  }
  outFile << "\n";

  outFile << "|---:|";
  for (std::size_t i = 0; i < report.rows.front().scores.size(); ++i) {
    outFile << "---:|";
  }
  outFile << "\n";

  outFile << std::fixed << std::setprecision(4);

  for (const auto &row : report.rows) {
    outFile << "| " << row.kmerLength << " |";
    for (const auto &score : row.scores) {
      outFile << " " << score.randIndex << " |";
    }
    outFile << "\n";
  }
  outFile << "\n";

  outFile << "## Notes\n\n";
  outFile << "- Rand Index compares predicted cluster labels with true class "
             "labels.\n";
  outFile << "- DBSCAN noise points are treated as cluster `-1`.\n";
  outFile << "- Higher Rand Index means better agreement with true classes.\n";
}