#include "cli/dispatcher/CommandDispatcher.hpp"
#include "cli/parser/ArgumentParser.hpp"

int main(int argc, char *argv[]) {
  CommandLineOptions options = ArgumentParser::parse(argc, argv);
  CommandDispatcher dispatcher;
  return dispatcher.dispatch(options);
}