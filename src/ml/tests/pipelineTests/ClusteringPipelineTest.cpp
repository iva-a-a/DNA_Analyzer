#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "../../app/ClusteringPipeline.hpp"
#include "../../common/Errors.hpp"

namespace {

void writeFile(const std::string &path, const std::string &content) {
  std::ofstream output(path);
  output << content;
}

std::string readFile(const std::string &path) {
  std::ifstream input(path);

  std::stringstream buffer;
  buffer << input.rdbuf();

  return buffer.str();
}

bool fileExists(const std::string &path) {
  std::ifstream input(path);
  return input.good();
}

std::string makeTestDatasetCsv() {
  return "sequence,class,species\n"
         "AAAAAA,0,class_a\n"
         "AAAATA,0,class_a\n"
         "CCCCCC,1,class_b\n"
         "CCCCAC,1,class_b\n"
         "GGGGGG,2,class_c\n"
         "GGGGAG,2,class_c\n";
}

ClusteringConfig makeBaseConfig(const std::string &inputPath,
                                const std::string &outputPath) {
  ClusteringConfig config;
  config.inputPath = inputPath;
  config.outputReportPath = outputPath;
  config.minKmerLength = 1;
  config.maxKmerLength = 2;
  config.randomSeed = 42;
  config.kmeansMaxIterations = 20;
  config.dbscanMinPoints = 2;
  config.kmedoidsMaxIterations = 20;
  return config;
}

} // namespace

TEST(ClusteringPipelineTest, RunCreatesMarkdownReport) {
  const std::string inputPath = "test_pipeline_dataset.csv";
  const std::string outputPath = "test_pipeline_report.md";

  writeFile(inputPath, makeTestDatasetCsv());

  ClusteringConfig config = makeBaseConfig(inputPath, outputPath);

  ClusteringPipeline pipeline;
  pipeline.run(config);

  ASSERT_TRUE(fileExists(outputPath));

  const std::string content = readFile(outputPath);

  EXPECT_NE(content.find("# DNA Clustering Report"), std::string::npos);
  EXPECT_NE(content.find("## Rand Index Results"), std::string::npos);
  EXPECT_NE(content.find("| K-mer | KMeans | DBSCAN | KMedoids |"),
            std::string::npos);
  EXPECT_NE(content.find("|---:|---:|---:|---:|"), std::string::npos);
  EXPECT_NE(content.find("| 1 |"), std::string::npos);
  EXPECT_NE(content.find("| 2 |"), std::string::npos);
  EXPECT_NE(content.find("## Notes"), std::string::npos);

  std::remove(inputPath.c_str());
  std::remove(outputPath.c_str());
}

TEST(ClusteringPipelineTest, RunUsesConfiguredKmerRange) {
  const std::string inputPath = "test_pipeline_kmer_range_dataset.csv";
  const std::string outputPath = "test_pipeline_kmer_range_report.md";

  writeFile(inputPath, makeTestDatasetCsv());

  ClusteringConfig config = makeBaseConfig(inputPath, outputPath);
  config.minKmerLength = 2;
  config.maxKmerLength = 3;

  ClusteringPipeline pipeline;
  pipeline.run(config);

  const std::string content = readFile(outputPath);

  EXPECT_EQ(content.find("| 1 |"), std::string::npos);
  EXPECT_NE(content.find("| 2 |"), std::string::npos);
  EXPECT_NE(content.find("| 3 |"), std::string::npos);

  std::remove(inputPath.c_str());
  std::remove(outputPath.c_str());
}

TEST(ClusteringPipelineTest, RunWritesAllConfiguredAlgorithms) {
  const std::string inputPath = "test_pipeline_algorithms_dataset.csv";
  const std::string outputPath = "test_pipeline_algorithms_report.md";

  writeFile(inputPath, makeTestDatasetCsv());

  ClusteringConfig config = makeBaseConfig(inputPath, outputPath);

  ClusteringPipeline pipeline;
  pipeline.run(config);

  const std::string content = readFile(outputPath);

  EXPECT_NE(content.find("KMeans"), std::string::npos);
  EXPECT_NE(content.find("DBSCAN"), std::string::npos);
  EXPECT_NE(content.find("KMedoids"), std::string::npos);

  std::remove(inputPath.c_str());
  std::remove(outputPath.c_str());
}

TEST(ClusteringPipelineTest, RunThrowsWhenInputFileDoesNotExist) {
  const std::string inputPath = "missing_pipeline_dataset.csv";
  const std::string outputPath = "test_pipeline_missing_input_report.md";

  ClusteringConfig config = makeBaseConfig(inputPath, outputPath);

  ClusteringPipeline pipeline;

  EXPECT_THROW(pipeline.run(config), FileReadError);

  std::remove(outputPath.c_str());
}

TEST(ClusteringPipelineTest, RunThrowsWhenOutputPathCannotBeOpened) {
  const std::string inputPath = "test_pipeline_bad_output_dataset.csv";
  const std::string outputPath = "/path/that/does/not/exist/report.md";

  writeFile(inputPath, makeTestDatasetCsv());

  ClusteringConfig config = makeBaseConfig(inputPath, outputPath);

  ClusteringPipeline pipeline;

  EXPECT_THROW(pipeline.run(config), ReportWriteError);

  std::remove(inputPath.c_str());
}

TEST(ClusteringPipelineTest, RunThrowsWhenKmerLengthIsTooLargeForAllSequences) {
  const std::string inputPath = "test_pipeline_large_k_dataset.csv";
  const std::string outputPath = "test_pipeline_large_k_report.md";

  writeFile(inputPath, makeTestDatasetCsv());

  ClusteringConfig config = makeBaseConfig(inputPath, outputPath);
  config.minKmerLength = 100;
  config.maxKmerLength = 100;

  ClusteringPipeline pipeline;

  EXPECT_THROW(pipeline.run(config), ValidationError);

  std::remove(inputPath.c_str());
  std::remove(outputPath.c_str());
}