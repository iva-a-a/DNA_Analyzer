CPP = g++
CPPFLAGS = -Wall -Werror -Wextra -std=c++20
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -coverage
RM = rm -rf

SRC_DIR = .
TEST_DIR = tests

BUILD_DIR = ../build
GCOV_DIR = ../gcov_report

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
TEST_FILES = $(shell find $(TEST_DIR) -name "*.cpp")

OBJ_FILES = $(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ_FILES = $(TEST_FILES:%.cpp=$(BUILD_DIR)/%.o)

TEST_BIN = $(BUILD_DIR)/tests_runner

all: test

test: $(TEST_BIN)
	$(TEST_BIN)

coverage: CPPFLAGS += $(GCOV_FLAGS)
coverage: clean test
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

$(TEST_BIN): $(OBJ_FILES) $(TEST_OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CPP) $(CPPFLAGS) $^ -o $@ $(GTEST_LIB_PATH) $(GTEST_LIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(GTEST_INCLUDE) -c $< -o $@