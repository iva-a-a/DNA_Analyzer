CPP = g++
CPPFLAGS = -Wall -Werror -Wextra -std=c++20
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -coverage
AR = ar
ARFLAGS = rcs
RM = rm -rf

SRC_DIR = .
TEST_DIR = tests

BUILD_DIR = ../build
GCOV_DIR = ../gcov_report

APP_BIN = $(BUILD_DIR)/app_runner
TEST_BIN = $(BUILD_DIR)/tests_runner
LIB_BIN = $(BUILD_DIR)/DNA_Analyzer.a

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

.PHONY: all clean tests test app library coverage create-test-files clean-test-files run-examples

all: library app tests

library: $(LIB_BIN)

app: $(APP_BIN)

tests: $(TEST_BIN)
	$(TEST_BIN)

test: tests

coverage: CPPFLAGS += $(GCOV_FLAGS)
coverage: clean tests
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

run-examples: app create-test-files
	./scripts/run_examples.sh

$(LIB_BIN): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

$(APP_BIN): $(LIB_BIN) $(MAIN_OBJ_FILE)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(MAIN_OBJ_FILE) $(LIB_BIN) -o $@

$(TEST_BIN): $(LIB_BIN) $(TEST_OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CPP) $(CPPFLAGS) $(TEST_OBJ_FILES) $(LIB_BIN) -o $@ $(GTEST_LIB_PATH) $(GTEST_LIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(GTEST_INCLUDE) -c $< -o $@