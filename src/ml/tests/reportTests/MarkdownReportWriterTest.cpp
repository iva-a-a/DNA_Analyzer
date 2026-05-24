#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../../common/Errors.hpp"
#include "../../report/MarkdownReportWriter.hpp"

namespace {

std::string readFile(const std::string &path) {
  std::ifstream input(path);

  std::stringstream buffer;
  buffer << input.rdbuf();

  return buffer.str();
}

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

TEST(MarkdownReportWriterTest,
     WritesReportWithHeaderScoresNotesAndConclusions) {
  const std::string outputPath = "test_clustering_report.md";

  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.73291),
                                       makeScore("DBSCAN", 0.20471),
                                       makeScore("KMedoids", 0.73531),
                                   }));

  report.rows.push_back(makeRow(2, {
                                       makeScore("KMeans", 0.72384),
                                       makeScore("DBSCAN", 0.28412),
                                       makeScore("KMedoids", 0.72435),
                                   }));

  MarkdownReportWriter writer;
  writer.write(outputPath, report);

  const std::string content = readFile(outputPath);

  const std::string expected =
      "# DNA Clustering Report\n"
      "\n"
      "## Rand Index Results\n"
      "\n"
      "| K-mer | KMeans | DBSCAN | KMedoids |\n"
      "|---:|---:|---:|---:|\n"
      "| 1 | 0.7329 | 0.2047 | 0.7353 |\n"
      "| 2 | 0.7238 | 0.2841 | 0.7244 |\n"
      "\n"
      "## Notes\n"
      "\n"
      "- Rand Index compares predicted cluster labels with true class labels.\n"
      "- DBSCAN noise points are treated as cluster `-1`.\n"
      "- Higher Rand Index means better agreement with true classes.\n"
      "\n"
      "## Conclusions\n"
      "\n"
      "- The best result was achieved by KMedoids with k-mer length 1, "
      "Rand Index = 0.7353.\n"
      "- Short k-mers produced better clustering quality. In this experiment, "
      "the strongest results were obtained for small k-mer lengths, especially "
      "k = 1 and k = 2.\n"
      "- As k-mer length increased, the quality generally decreased. This is "
      "likely because longer k-mers create a larger and more sparse feature "
      "space, so sequences have fewer shared features.\n"
      "- KMeans and KMedoids were more stable than DBSCAN across different "
      "k-mer lengths.\n"
      "- DBSCAN was more sensitive to the choice of k-mer length and eps "
      "parameter, because it relies on density in the feature space rather "
      "than a predefined number of clusters.";

  EXPECT_EQ(content, expected);

  std::remove(outputPath.c_str());
}

TEST(MarkdownReportWriterTest, WritesMessageWhenReportIsEmpty) {
  const std::string outputPath = "test_empty_clustering_report.md";

  ClusteringReport report;

  MarkdownReportWriter writer;
  writer.write(outputPath, report);

  const std::string content = readFile(outputPath);

  const std::string expected =
      "# DNA Clustering Report\n"
      "\n"
      "## Rand Index Results\n"
      "\n"
      "No results available.\n"
      "\n"
      "## Notes\n"
      "\n"
      "- Rand Index compares predicted cluster labels with true class labels.\n"
      "- DBSCAN noise points are treated as cluster `-1`.\n"
      "- Higher Rand Index means better agreement with true classes.\n";

  EXPECT_EQ(content, expected);

  std::remove(outputPath.c_str());
}

TEST(MarkdownReportWriterTest, ThrowsWhenOutputFileCannotBeOpened) {
  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.7),
                                   }));

  MarkdownReportWriter writer;

  EXPECT_THROW(writer.write("/path/that/does/not/exist/report.md", report),
               ReportWriteError);
}

TEST(MarkdownReportWriterTest, UsesAlgorithmsFromFirstRowAsTableHeader) {
  const std::string outputPath = "test_header_clustering_report.md";

  ClusteringReport report;
  report.rows.push_back(makeRow(3, {
                                       makeScore("KMeans", 0.724),
                                       makeScore("DBSCAN", 0.5868),
                                   }));

  MarkdownReportWriter writer;
  writer.write(outputPath, report);

  const std::string content = readFile(outputPath);

  EXPECT_NE(content.find("| K-mer | KMeans | DBSCAN |"), std::string::npos);
  EXPECT_NE(content.find("|---:|---:|---:|"), std::string::npos);
  EXPECT_NE(content.find("| 3 | 0.7240 | 0.5868 |"), std::string::npos);

  std::remove(outputPath.c_str());
}

TEST(MarkdownReportWriterTest, FormatsRandIndexWithFourDecimalPlaces) {
  const std::string outputPath = "test_precision_clustering_report.md";

  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 1.0),
                                       makeScore("DBSCAN", 0.123456),
                                       makeScore("KMedoids", 0.7),
                                   }));

  MarkdownReportWriter writer;
  writer.write(outputPath, report);

  const std::string content = readFile(outputPath);

  EXPECT_NE(content.find("| 1 | 1.0000 | 0.1235 | 0.7000 |"),
            std::string::npos);
  EXPECT_NE(content.find("Rand Index = 1.0000"), std::string::npos);

  std::remove(outputPath.c_str());
}

TEST(MarkdownReportWriterTest, ConclusionsUseBestScoreFromReport) {
  const std::string outputPath = "test_conclusions_best_score_report.md";

  ClusteringReport report;
  report.rows.push_back(makeRow(1, {
                                       makeScore("KMeans", 0.6),
                                       makeScore("DBSCAN", 0.5),
                                       makeScore("KMedoids", 0.7),
                                   }));

  report.rows.push_back(makeRow(2, {
                                       makeScore("KMeans", 0.9),
                                       makeScore("DBSCAN", 0.4),
                                       makeScore("KMedoids", 0.8),
                                   }));

  MarkdownReportWriter writer;
  writer.write(outputPath, report);

  const std::string content = readFile(outputPath);

  EXPECT_NE(content.find("## Conclusions"), std::string::npos);
  EXPECT_NE(content.find("The best result was achieved by KMeans "
                         "with k-mer length 2, Rand Index = 0.9000."),
            std::string::npos);

  std::remove(outputPath.c_str());
}