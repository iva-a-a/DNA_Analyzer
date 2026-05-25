DNA_DIR = algorithms
ML_DIR = ml

RM = rm -rf
BUILD_DIR = build
GCOV_DIR = gcov_report

.PHONY: all clean tests test app library coverage run-examples check-limits clean-all-files

all:
	$(MAKE) -C $(DNA_DIR) all
	$(MAKE) -C $(ML_DIR) all

app:
	$(MAKE) -C $(DNA_DIR) app
	$(MAKE) -C $(ML_DIR) app

tests:
	$(MAKE) -C $(DNA_DIR) tests
	$(MAKE) -C $(ML_DIR) tests

test: tests

library:
	$(MAKE) -C $(DNA_DIR) library
	$(MAKE) -C $(ML_DIR) library

coverage:
	$(MAKE) -C $(DNA_DIR) coverage
	$(MAKE) -C $(ML_DIR) coverage

clean:
	$(MAKE) -C $(DNA_DIR) clean
	$(MAKE) -C $(ML_DIR) clean
	$(RM) $(BUILD_DIR)
	$(RM) $(GCOV_DIR)

run-examples:
	$(MAKE) -C $(DNA_DIR) run-examples

check-limits:
	$(MAKE) -C $(DNA_DIR) check-limits

clean-all-files:
	$(MAKE) -C $(DNA_DIR) clean-all-files



