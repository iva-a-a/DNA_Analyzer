#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../../common/Errors.hpp"
#include "../../data/loader/DatasetLoader.hpp"

namespace {

std::filesystem::path makeTempFilePath(const std::string &fileName) {
  return std::filesystem::temp_directory_path() / fileName;
}

void writeFile(const std::filesystem::path &path, const std::string &content) {
  std::ofstream file(path);
  ASSERT_TRUE(file.is_open());
  file << content;
}

void removeFileIfExists(const std::filesystem::path &path) {
  std::error_code ec;
  std::filesystem::remove(path, ec);
}

} // namespace

TEST(DatasetLoaderTest, LoadsValidCsvFile) {
  const auto path = makeTempFilePath("ml_dataset_loader_valid.csv");
  writeFile(path, "sequence,class,species\nACGT,0,human\nTGCA,1,dog");

  DnaDataset dataset = DatasetLoader::load(path.string());

  EXPECT_EQ(dataset.size(), 2U);
  EXPECT_EQ(dataset.records()[0].sequence, "ACGT");
  EXPECT_EQ(dataset.records()[1].species, "dog");

  removeFileIfExists(path);
}

TEST(DatasetLoaderTest, ThrowsFileReadErrorWhenFileDoesNotExist) {
  const auto path = makeTempFilePath("ml_dataset_loader_missing.csv");
  removeFileIfExists(path);

  EXPECT_THROW(DatasetLoader::load(path.string()), FileReadError);
}

TEST(DatasetLoaderTest, ThrowsValidationErrorForInvalidDnaInFile) {
  const auto path = makeTempFilePath("ml_dataset_loader_invalid_dna.csv");
  writeFile(path, "sequence,class,species\nACGX,0,human");

  EXPECT_THROW(DatasetLoader::load(path.string()), ValidationError);

  removeFileIfExists(path);
}

TEST(DatasetLoaderTest, ThrowsInputFormatErrorForMalformedCsv) {
  const auto path = makeTempFilePath("ml_dataset_loader_bad_csv.csv");
  writeFile(path, "sequence,class,species\nonly,two");

  EXPECT_THROW(DatasetLoader::load(path.string()), InputFormatError);

  removeFileIfExists(path);
}

TEST(DatasetLoaderTest, ThrowsValidationErrorForEmptyDatasetFile) {
  const auto path = makeTempFilePath("ml_dataset_loader_header_only.csv");
  writeFile(path, "sequence,class,species\n");

  EXPECT_THROW(DatasetLoader::load(path.string()), ValidationError);

  removeFileIfExists(path);
}

TEST(DatasetLoaderTest, ThrowsValidationErrorForNegativeClassId) {
  const auto path = makeTempFilePath("ml_dataset_loader_negative_class.csv");
  writeFile(path, "sequence,class,species\nACGT,-1,human");

  EXPECT_THROW(DatasetLoader::load(path.string()), ValidationError);

  removeFileIfExists(path);
}
