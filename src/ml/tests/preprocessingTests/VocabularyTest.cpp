#include <gtest/gtest.h>

#include "../../common/Errors.hpp"
#include "../../preprocessing/Vocabulary.hpp"

namespace {

std::vector<std::vector<std::string>> makeDocuments() {
  return {
      {"AC", "CG"},
      {"CG", "GT"},
  };
}

} // namespace

TEST(VocabularyTest, FitBuildsUniqueTokensInEncounterOrder) {
  Vocabulary vocabulary;
  vocabulary.fit(makeDocuments());

  EXPECT_EQ(vocabulary.size(), 3U);
  EXPECT_EQ(vocabulary.tokens(), (std::vector<std::string>{"AC", "CG", "GT"}));
}

TEST(VocabularyTest, ContainsReturnsTrueForKnownTokens) {
  Vocabulary vocabulary;
  vocabulary.fit(makeDocuments());

  EXPECT_TRUE(vocabulary.contains("AC"));
  EXPECT_TRUE(vocabulary.contains("CG"));
  EXPECT_TRUE(vocabulary.contains("GT"));
  EXPECT_FALSE(vocabulary.contains("TA"));
}

TEST(VocabularyTest, IndexOfReturnsCorrectIndex) {
  Vocabulary vocabulary;
  vocabulary.fit(makeDocuments());

  EXPECT_EQ(vocabulary.indexOf("AC"), 0U);
  EXPECT_EQ(vocabulary.indexOf("CG"), 1U);
  EXPECT_EQ(vocabulary.indexOf("GT"), 2U);
}

TEST(VocabularyTest, IndexOfThrowsForUnknownToken) {
  Vocabulary vocabulary;
  vocabulary.fit(makeDocuments());

  EXPECT_THROW(vocabulary.indexOf("XX"), ValidationError);
}

TEST(VocabularyTest, FitOnEmptyDocumentsLeavesVocabularyEmpty) {
  Vocabulary vocabulary;
  vocabulary.fit({});

  EXPECT_EQ(vocabulary.size(), 0U);
  EXPECT_FALSE(vocabulary.contains("AC"));
}

TEST(VocabularyTest, RefitReplacesPreviousVocabulary) {
  Vocabulary vocabulary;
  vocabulary.fit(makeDocuments());
  vocabulary.fit({{"AA"}, {"BB"}});

  EXPECT_EQ(vocabulary.size(), 2U);
  EXPECT_TRUE(vocabulary.contains("AA"));
  EXPECT_FALSE(vocabulary.contains("AC"));
}

TEST(VocabularyTest, FitIgnoresDuplicateTokensWithinDocument) {
  Vocabulary vocabulary;
  vocabulary.fit({{"AC", "AC", "AC"}});

  EXPECT_EQ(vocabulary.size(), 1U);
  EXPECT_EQ(vocabulary.indexOf("AC"), 0U);
}

TEST(VocabularyTest, TokensReturnsConstReferenceToInternalStorage) {
  Vocabulary vocabulary;
  vocabulary.fit({{"AC"}});

  const std::vector<std::string> &tokens = vocabulary.tokens();

  EXPECT_EQ(tokens.size(), 1U);
  EXPECT_EQ(&tokens, &vocabulary.tokens());
}
