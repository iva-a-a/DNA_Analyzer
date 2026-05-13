#include <gtest/gtest.h>

#include "../../cli/parser/ArgumentParser.hpp"

TEST(ArgumentParserParseTest, ReturnsUnknownWhenNoArguments) {
  char *argv[] = {
      const_cast<char *>("program"),
  };

  const CommandLineOptions options = ArgumentParser::parse(1, argv);

  EXPECT_EQ(options.type, CommandType::Unknown);
  EXPECT_TRUE(options.arguments.empty());
}

TEST(ArgumentParserParseTest, ParsesExactSearchCommand) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("exact-search"),
  };

  const CommandLineOptions options = ArgumentParser::parse(2, argv);

  EXPECT_EQ(options.type, CommandType::ExactSearch);
  EXPECT_TRUE(options.arguments.empty());
}

TEST(ArgumentParserParseTest, ParsesAlignScoreCommand) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("align-score"),
  };

  const CommandLineOptions options = ArgumentParser::parse(2, argv);

  EXPECT_EQ(options.type, CommandType::AlignScore);
}

TEST(ArgumentParserParseTest, ParsesAlignCommand) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("align"),
  };

  const CommandLineOptions options = ArgumentParser::parse(2, argv);

  EXPECT_EQ(options.type, CommandType::Align);
}

TEST(ArgumentParserParseTest, ParsesRegexMatchCommand) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("regex-match"),
  };

  const CommandLineOptions options = ArgumentParser::parse(2, argv);

  EXPECT_EQ(options.type, CommandType::RegexMatch);
}

TEST(ArgumentParserParseTest, ParsesKSimilarityCommand) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("k-similarity"),
  };

  const CommandLineOptions options = ArgumentParser::parse(2, argv);

  EXPECT_EQ(options.type, CommandType::KSimilarity);
}

TEST(ArgumentParserParseTest, ParsesMinWindowCommand) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("min-window"),
  };

  const CommandLineOptions options = ArgumentParser::parse(2, argv);

  EXPECT_EQ(options.type, CommandType::MinWindow);
}

TEST(ArgumentParserParseTest, ReturnsUnknownForUnsupportedCommand) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("unsupported-command"),
  };

  const CommandLineOptions options = ArgumentParser::parse(2, argv);

  EXPECT_EQ(options.type, CommandType::Unknown);
  EXPECT_TRUE(options.arguments.empty());
}

TEST(ArgumentParserParseTest, CollectsSingleArgument) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("exact-search"),
      const_cast<char *>("input.txt"),
  };

  const CommandLineOptions options = ArgumentParser::parse(3, argv);

  EXPECT_EQ(options.type, CommandType::ExactSearch);

  const std::vector<std::string> expected = {
      "input.txt",
  };

  EXPECT_EQ(options.arguments, expected);
}

TEST(ArgumentParserParseTest, CollectsMultipleArgumentsInOrder) {
  char *argv[] = {
      const_cast<char *>("program"),   const_cast<char *>("align"),
      const_cast<char *>("file1.txt"), const_cast<char *>("file2.txt"),
      const_cast<char *>("--verbose"),
  };

  const CommandLineOptions options = ArgumentParser::parse(5, argv);

  EXPECT_EQ(options.type, CommandType::Align);

  const std::vector<std::string> expected = {
      "file1.txt",
      "file2.txt",
      "--verbose",
  };

  EXPECT_EQ(options.arguments, expected);
}

TEST(ArgumentParserParseTest, PreservesEmptyArgument) {
  char emptyArg[] = "";

  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("exact-search"),
      emptyArg,
  };

  const CommandLineOptions options = ArgumentParser::parse(3, argv);

  EXPECT_EQ(options.type, CommandType::ExactSearch);

  const std::vector<std::string> expected = {
      "",
  };

  EXPECT_EQ(options.arguments, expected);
}

TEST(ArgumentParserParseTest, PreservesArgumentsWithSpaces) {
  char argWithSpaces[] = "file name.txt";

  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("align"),
      argWithSpaces,
  };

  const CommandLineOptions options = ArgumentParser::parse(3, argv);

  EXPECT_EQ(options.type, CommandType::Align);

  const std::vector<std::string> expected = {
      "file name.txt",
  };

  EXPECT_EQ(options.arguments, expected);
}

TEST(ArgumentParserParseTest, UnknownCommandStillCollectsArguments) {
  char *argv[] = {
      const_cast<char *>("program"),
      const_cast<char *>("unknown"),
      const_cast<char *>("arg1"),
      const_cast<char *>("arg2"),
  };

  const CommandLineOptions options = ArgumentParser::parse(4, argv);

  EXPECT_EQ(options.type, CommandType::Unknown);

  const std::vector<std::string> expected = {
      "arg1",
      "arg2",
  };

  EXPECT_EQ(options.arguments, expected);
}

TEST(ArgumentParserParseTest, HandlesLargeNumberOfArguments) {
  char *argv[] = {
      const_cast<char *>("program"), const_cast<char *>("min-window"),
      const_cast<char *>("a"),       const_cast<char *>("b"),
      const_cast<char *>("c"),       const_cast<char *>("d"),
      const_cast<char *>("e"),       const_cast<char *>("f"),
      const_cast<char *>("g"),       const_cast<char *>("h"),
  };

  const CommandLineOptions options = ArgumentParser::parse(10, argv);

  EXPECT_EQ(options.type, CommandType::MinWindow);

  const std::vector<std::string> expected = {
      "a", "b", "c", "d", "e", "f", "g", "h",
  };

  EXPECT_EQ(options.arguments, expected);
}