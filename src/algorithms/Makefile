CPP = g++
CPPFLAGS = -Wall -Werror -Wextra -std=c++20
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -coverage
AR = ar
ARFLAGS = rcs
RM = rm -rf

SRC_DIR = .
TEST_DIR = tests

BUILD_DIR = ../build/dna
GCOV_DIR = ../gcov_report/dna

APP_BIN = $(BUILD_DIR)/dna_runner
TEST_BIN = $(BUILD_DIR)/dna_tests_runner
LIB_BIN = $(BUILD_DIR)/DNA_Analyzer.a

COVERAGE_FLAGS =

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	GTEST_PREFIX := $(shell brew --prefix googletest)
	GTEST_INCLUDE = -I$(GTEST_PREFIX)/include
	GTEST_LIB_PATH = -L$(GTEST_PREFIX)/lib
	GTEST_LIBS = -lgtest -pthread
	OPEN_CMD = open
else
	GTEST_INCLUDE =
	GTEST_LIB_PATH =
	GTEST_LIBS = -lgtest -pthread
	OPEN_CMD = xdg-open
endif

SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp" ! -path "./$(TEST_DIR)/*" ! -name "main.cpp")
MAIN_FILE = $(SRC_DIR)/main.cpp
TEST_FILES = $(shell find $(TEST_DIR) -name "*.cpp")

OBJ_FILES = $(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJ_FILE = $(MAIN_FILE:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ_FILES = $(TEST_FILES:%.cpp=$(BUILD_DIR)/%.o)

LIB_CPPFLAGS = $(CPPFLAGS) $(COVERAGE_FLAGS)
APP_CPPFLAGS = $(CPPFLAGS)
TEST_CPPFLAGS = $(CPPFLAGS) $(GTEST_INCLUDE) $(COVERAGE_FLAGS)
TEST_LDFLAGS = $(GTEST_LIB_PATH) $(GTEST_LIBS) $(COVERAGE_FLAGS)

.PHONY: all clean tests test app library coverage create-test-files clean-all-files run-examples check-limits

all: library app tests

library: $(LIB_BIN)

app: $(APP_BIN)

tests: $(TEST_BIN)
	$(TEST_BIN)

test: tests

coverage: COVERAGE_FLAGS = $(GCOV_FLAGS)
coverage: clean $(TEST_BIN)
	$(TEST_BIN)
	@mkdir -p $(GCOV_DIR)
	gcovr \
		--root . \
		--object-directory $(BUILD_DIR) \
		--html \
		--html-details \
		--output $(GCOV_DIR)/index.html \
		--exclude 'tests/.*'
	$(OPEN_CMD) $(GCOV_DIR)/index.html

clean:
	$(RM) $(BUILD_DIR)
	$(RM) $(GCOV_DIR)
	$(RM) *.gcda *.gcno *.gcov

create-test-files:
	./scripts/create_test_files.sh

clean-test-files:
	./scripts/clean_test_files.sh

run-examples: clean app create-test-files
	./scripts/run_examples.sh

create-large-test-files:
	./scripts/create_large_test_files.sh

clean-large-test-files:
	./scripts/clean_large_test_files.sh

check-limits: app
	./scripts/create_large_test_files.sh
	./scripts/check_limits.sh

clean-all-files: clean-test-files clean-large-test-files

$(LIB_BIN): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

$(APP_BIN): $(LIB_BIN) $(MAIN_OBJ_FILE)
	@mkdir -p $(dir $@)
	$(CPP) $(APP_CPPFLAGS) $(MAIN_OBJ_FILE) $(LIB_BIN) -o $@

$(TEST_BIN): $(LIB_BIN) $(TEST_OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CPP) $(CPPFLAGS) $(TEST_OBJ_FILES) $(LIB_BIN) -o $@ $(TEST_LDFLAGS)

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(TEST_CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(APP_CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(LIB_CPPFLAGS) -c $< -o $@