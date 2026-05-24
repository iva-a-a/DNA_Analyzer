#include "ClusteringReportAnalyzer.hpp"

#include "../common/Errors.hpp"

namespace ClusteringReportAnalyzer {

ClusteringReportAnalysis analyze(const ClusteringReport &report) {
  if (report.rows.empty()) {
    throw ReportWriteError("Cannot analyze empty clustering report");
  }

  if (report.rows.front().scores.empty()) {
    throw ReportWriteError("Cannot analyze clustering report without scores");
  }

  ClusteringReportAnalysis analysis;
  analysis.bestKmerLength = report.rows.front().kmerLength;
  analysis.bestAlgorithm = report.rows.front().scores.front().algorithmName;
  analysis.bestRandIndex = report.rows.front().scores.front().randIndex;

  for (const auto &row : report.rows) {
    for (const auto &score : row.scores) {
      if (score.randIndex > analysis.bestRandIndex) {
        analysis.bestRandIndex = score.randIndex;
        analysis.bestAlgorithm = score.algorithmName;
        analysis.bestKmerLength = row.kmerLength;
      }
    }
  }

  return analysis;
}

} // namespace ClusteringReportAnalyzer