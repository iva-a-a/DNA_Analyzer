#include "ClusteringPipeline.hpp"

#include <memory>

#include "../clustering/DbscanClusterer.hpp"
#include "../clustering/FixedTableEpsProvider.hpp"
#include "../clustering/IClusterer.hpp"
#include "../clustering/KMeansClusterer.hpp"
#include "../clustering/KMedoidsClusterer.hpp"
#include "../data/loader/DatasetLoader.hpp"
#include "../metrics/RandIndex.hpp"
#include "../preprocessing/Vectorizer.hpp"
#include "../report/ClusteringReport.hpp"
#include "../report/ClusteringReportRow.hpp"
#include "../report/ClusteringScore.hpp"
#include "../report/MarkdownReportWriter.hpp"

void ClusteringPipeline::run(const ClusteringConfig &config) const {
  DnaDataset dataset = DatasetLoader::load(config.inputPath);

  const std::vector<std::string> sequences = dataset.sequences();
  const std::vector<int> classIds = dataset.classIds();
  const int classCount = dataset.classCount();

  std::vector<std::unique_ptr<IClusterer>> clusterers;

  clusterers.push_back(std::make_unique<KMeansClusterer>(
      config.kmeansMaxIterations, config.randomSeed));

  clusterers.push_back(std::make_unique<DbscanClusterer>(
      std::make_shared<FixedTableEpsProvider>(), config.dbscanMinPoints));

  clusterers.push_back(std::make_unique<KMedoidsClusterer>(
      config.kmedoidsMaxIterations, config.randomSeed));

  Vectorizer vectorizer;
  RandIndex randIndex;
  ClusteringReport report;

  for (int kmerLength = config.minKmerLength;
       kmerLength <= config.maxKmerLength; ++kmerLength) {
    const std::vector<std::vector<double>> vectors = vectorizer.fitTransform(
        sequences, static_cast<std::size_t>(kmerLength));

    ClusteringContext context;
    context.expectedClusterCount = classCount;
    context.kmerLength = kmerLength;

    ClusteringReportRow row;
    row.kmerLength = kmerLength;

    for (const auto &clusterer : clusterers) {
      const ClusteringResult result = clusterer->fitPredict(vectors, context);

      ClusteringScore score;
      score.algorithmName = clusterer->name();
      score.randIndex = randIndex.compute(classIds, result.labels);
      row.scores.push_back(score);
    }

    report.rows.push_back(row);
  }

  MarkdownReportWriter().write(config.outputReportPath, report);
}
