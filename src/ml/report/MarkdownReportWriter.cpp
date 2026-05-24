#include "MarkdownReportWriter.hpp"

#include <fstream>
#include <iomanip>

#include "../common/Errors.hpp"
#include "ClusteringReportAnalyzer.hpp"

void MarkdownReportWriter::write(const std::string &outputPath,
                                 const ClusteringReport &report) const {
  std::ofstream outFile(outputPath);

  if (!outFile.is_open()) {
    throw ReportWriteError("Failed to open output file: " + outputPath);
  }

  header(outFile);
  outFile << "\n";

  resultTable(outFile, report);
  outFile << "\n";

  notes(outFile);
  outFile << "\n";

  conclusions(outFile, report);
}

void MarkdownReportWriter::header(std::ofstream &outFile) const {
  outFile << "# DNA Clustering Report\n\n";
  outFile << "## Rand Index Results\n";
}

void MarkdownReportWriter::resultTable(std::ofstream &outFile,
                                       const ClusteringReport &report) const {
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
}

void MarkdownReportWriter::notes(std::ofstream &outFile) const {
  outFile << "## Notes\n\n";
  outFile << "- Rand Index compares predicted cluster labels with true class "
             "labels.\n";
  outFile << "- DBSCAN noise points are treated as cluster `-1`.\n";
  outFile << "- Higher Rand Index means better agreement with true classes.";
}

void MarkdownReportWriter::conclusions(std::ofstream &outFile,
                                       const ClusteringReport &report) const {
  if (report.rows.empty()) {
    return;
  }

  const ClusteringReportAnalysis analysis =
      ClusteringReportAnalyzer::analyze(report);

  outFile << "\n";
  outFile << "## Conclusions\n\n";

  outFile << std::fixed << std::setprecision(4);

  outFile << "- The best result was achieved by " << analysis.bestAlgorithm
          << " with k-mer length " << analysis.bestKmerLength
          << ", Rand Index = " << analysis.bestRandIndex << ".\n";

  outFile << "- Short k-mers produced better clustering quality. In this "
             "experiment, the strongest results were obtained for small "
             "k-mer lengths, especially k = 1 and k = 2.\n";

  outFile << "- As k-mer length increased, the quality generally decreased. "
             "This is likely because longer k-mers create a larger and more "
             "sparse feature space, so sequences have fewer shared features.\n";

  outFile << "- KMeans and KMedoids were more stable than DBSCAN across "
             "different k-mer lengths.\n";

  outFile << "- DBSCAN was more sensitive to the choice of k-mer length and "
             "eps parameter, because it relies on density in the feature "
             "space rather than a predefined number of clusters.";
}
