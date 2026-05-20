#include <gtest/gtest.h>

#include <sstream>

#include "../../common/Errors.hpp"
#include "../../data/parser/DatasetParser.hpp"

namespace {

std::istringstream makeStream(const std::string &content) {
  return std::istringstream(content);
}

} // namespace

TEST(DatasetParserTest, ParsesValidCsvWithHeader) {
  auto stream = makeStream("sequence,class,species\nACGT,0,human\nTGCA,1,dog");

  DnaDataset dataset = DatasetParser::parse(stream);

  ASSERT_EQ(dataset.size(), 2U);
  EXPECT_EQ(dataset.records()[0].sequence, "ACGT");
  EXPECT_EQ(dataset.records()[0].classId, 0);
  EXPECT_EQ(dataset.records()[0].species, "human");
  EXPECT_EQ(dataset.records()[1].sequence, "TGCA");
  EXPECT_EQ(dataset.records()[1].classId, 1);
  EXPECT_EQ(dataset.records()[1].species, "dog");
}

TEST(DatasetParserTest, SkipsEmptyLines) {
  auto stream = makeStream(
      "sequence,class,species\n\nACGT,0,human\n\nTGCA,1,dog\n");

  DnaDataset dataset = DatasetParser::parse(stream);

  EXPECT_EQ(dataset.size(), 2U);
}

TEST(DatasetParserTest, ParsesFileWithOnlyHeaderReturnsEmptyDataset) {
  auto stream = makeStream("sequence,class,species\n");

  DnaDataset dataset = DatasetParser::parse(stream);

  EXPECT_TRUE(dataset.empty());
}

TEST(DatasetParserTest, ParsesEmptyStreamReturnsEmptyDataset) {
  auto stream = makeStream("");

  DnaDataset dataset = DatasetParser::parse(stream);

  EXPECT_TRUE(dataset.empty());
}

TEST(DatasetParserTest, ThrowsInputFormatErrorWhenColumnCountIsWrong) {
  auto stream = makeStream("sequence,class,species\nACGT,0");

  EXPECT_THROW(DatasetParser::parse(stream), InputFormatError);
}

TEST(DatasetParserTest, ThrowsInputFormatErrorWhenTooManyColumns) {
  auto stream = makeStream("sequence,class,species\nACGT,0,human,extra");

  EXPECT_THROW(DatasetParser::parse(stream), InputFormatError);
}

TEST(DatasetParserTest, ThrowsInputFormatErrorWhenClassIdIsNotInteger) {
  auto stream = makeStream("sequence,class,species\nACGT,abc,human");

  EXPECT_THROW(DatasetParser::parse(stream), InputFormatError);
}

TEST(DatasetParserTest, ThrowsInputFormatErrorWithLineNumberForInvalidFormat) {
  auto stream = makeStream("sequence,class,species\nACGT,0,human\nBAD,row");

  try {
    DatasetParser::parse(stream);
    FAIL() << "Expected InputFormatError";
  } catch (const InputFormatError &error) {
    EXPECT_NE(std::string(error.what()).find("line 3"), std::string::npos);
  }
}

TEST(DatasetParserTest, ThrowsInputFormatErrorWithLineNumberForInvalidClassId) {
  auto stream = makeStream("sequence,class,species\nACGT,x,human");

  try {
    DatasetParser::parse(stream);
    FAIL() << "Expected InputFormatError";
  } catch (const InputFormatError &error) {
    EXPECT_NE(std::string(error.what()).find("line 2"), std::string::npos);
  }
}

TEST(DatasetParserTest, AcceptsNegativeClassIdInParser) {
  auto stream = makeStream("sequence,class,species\nACGT,-1,human");

  DnaDataset dataset = DatasetParser::parse(stream);

  ASSERT_EQ(dataset.size(), 1U);
  EXPECT_EQ(dataset.records()[0].classId, -1);
}

TEST(DatasetParserTest, AcceptsNInSequenceAtParseStage) {
  auto stream = makeStream("sequence,class,species\nACGTN,0,human");

  DnaDataset dataset = DatasetParser::parse(stream);

  ASSERT_EQ(dataset.size(), 1U);
  EXPECT_EQ(dataset.records()[0].sequence, "ACGTN");
}
