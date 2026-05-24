#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "../../common/Errors.hpp"
#include "../../report/ClusteringReportAnalyzer.hpp"

namespace {

ClusteringScore makeScore(const std::string &algorithmName, double randIndex) {
  ClusteringScore score;
  score.algorithmName = algorithmName;
  score.randIndex = randIndex;
  return score;
}

ClusteringReportRow makeRow(int kmerLength,
                            const std::vector<ClusteringScore> &scores) {
  ClusteringReportRow row;
  row.kmerLength = kmerLength;
  row.scores = scores;
  return row;
}

} // namespace

TEST(ClusteringReportAnalyzerTest, FindsBestScoreInSingleRow) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.7329),
                                       makeScore("DBSCAN", 0.6876),
                                       makeScore("KMedoids", 0.7353),
                                   }));

  const ClusteringReportAnalysis analysis =
      ClusteringReportAnalyzer::analyze(report);

  EXPECT_EQ(analysis.bestKmerLength, 1);
  EXPECT_EQ(analysis.bestAlgorithm, "KMedoids");
  EXPECT_DOUBLE_EQ(analysis.bestRandIndex, 0.7353);
}

TEST(ClusteringReportAnalyzerTest, FindsBestScoreAcrossMultipleRows) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.7329),
                                       makeScore("DBSCAN", 0.6876),
                                       makeScore("KMedoids", 0.7353),
                                   }));

  report.rows.push_back(makeRow(2, {
                                       makeScore("KMeans", 0.7238),
                                       makeScore("DBSCAN", 0.5884),
                                       makeScore("KMedoids", 0.7243),
                                   }));

  report.rows.push_back(makeRow(3, {
                                       makeScore("KMeans", 0.7240),
                                       makeScore("DBSCAN", 0.5339),
                                       makeScore("KMedoids", 0.7138),
                                   }));

  const ClusteringReportAnalysis analysis =
      ClusteringReportAnalyzer::analyze(report);

  EXPECT_EQ(analysis.bestKmerLength, 1);
  EXPECT_EQ(analysis.bestAlgorithm, "KMedoids");
  EXPECT_DOUBLE_EQ(analysis.bestRandIndex, 0.7353);
}

TEST(ClusteringReportAnalyzerTest, FindsBestScoreWhenBestIsInLaterRow) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.5),
                                       makeScore("DBSCAN", 0.4),
                                   }));

  report.rows.push_back(makeRow(2, {
                                       makeScore("KMeans", 0.9),
                                       makeScore("DBSCAN", 0.3),
                                   }));

  const ClusteringReportAnalysis analysis =
      ClusteringReportAnalyzer::analyze(report);

  EXPECT_EQ(analysis.bestKmerLength, 2);
  EXPECT_EQ(analysis.bestAlgorithm, "KMeans");
  EXPECT_DOUBLE_EQ(analysis.bestRandIndex, 0.9);
}

TEST(ClusteringReportAnalyzerTest, KeepsFirstBestScoreWhenValuesAreEqual) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.8),
                                       makeScore("DBSCAN", 0.8),
                                   }));

  report.rows.push_back(makeRow(2, {
                                       makeScore("KMedoids", 0.8),
                                   }));

  const ClusteringReportAnalysis analysis =
      ClusteringReportAnalyzer::analyze(report);

  EXPECT_EQ(analysis.bestKmerLength, 1);
  EXPECT_EQ(analysis.bestAlgorithm, "KMeans");
  EXPECT_DOUBLE_EQ(analysis.bestRandIndex, 0.8);
}

TEST(ClusteringReportAnalyzerTest, ThrowsWhenReportIsEmpty) {
  ClusteringReport report;

  EXPECT_THROW(ClusteringReportAnalyzer::analyze(report), ReportWriteError);
}

TEST(ClusteringReportAnalyzerTest, ThrowsWhenFirstRowHasNoScores) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {}));

  EXPECT_THROW(ClusteringReportAnalyzer::analyze(report), ReportWriteError);
}

TEST(ClusteringReportAnalyzerTest, IgnoresEmptyRowsAfterFirstRow) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.7),
                                   }));

  report.rows.push_back(makeRow(2, {}));

  const ClusteringReportAnalysis analysis =
      ClusteringReportAnalyzer::analyze(report);

  EXPECT_EQ(analysis.bestKmerLength, 1);
  EXPECT_EQ(analysis.bestAlgorithm, "KMeans");
  EXPECT_DOUBLE_EQ(analysis.bestRandIndex, 0.7);
}

TEST(ClusteringReportAnalyzerTest, HandlesNegativeRandIndexValues) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("AlgorithmA", -0.5),
                                       makeScore("AlgorithmB", -0.2),
                                   }));

  const ClusteringReportAnalysis analysis =
      ClusteringReportAnalyzer::analyze(report);

  EXPECT_EQ(analysis.bestKmerLength, 1);
  EXPECT_EQ(analysis.bestAlgorithm, "AlgorithmB");
  EXPECT_DOUBLE_EQ(analysis.bestRandIndex, -0.2);
}

TEST(ClusteringReportAnalysisTest, DefaultValuesAreInitialized) {
  ClusteringReportAnalysis analysis;

  EXPECT_EQ(analysis.bestKmerLength, 0);
  EXPECT_TRUE(analysis.bestAlgorithm.empty());
  EXPECT_DOUBLE_EQ(analysis.bestRandIndex, 0.0);
}