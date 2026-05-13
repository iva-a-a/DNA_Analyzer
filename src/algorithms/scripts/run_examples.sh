#!/usr/bin/env bash

set -e

APP="../build/app_runner"
DATA_DIR="test_data"

if [ ! -x "$APP" ]; then
  echo "Application binary not found: $APP"
  echo "Run: make app"
  exit 1
fi

if [ ! -d "$DATA_DIR" ]; then
  echo "Test data directory not found: $DATA_DIR"
  echo "Run: ./scripts/create_test_files.sh"
  exit 1
fi

echo "Exact search:"
"$APP" exact-search "$DATA_DIR/exact_text.txt" "$DATA_DIR/exact_pattern.txt"
echo

echo "Exact search, no matches:"
"$APP" exact-search "$DATA_DIR/exact_no_match_text.txt" "$DATA_DIR/exact_no_match_pattern.txt"
echo

echo "Alignment score:"
"$APP" align-score "$DATA_DIR/align_score_input.txt"
echo

echo "Full alignment:"
"$APP" align "$DATA_DIR/align_input.txt"
echo

echo "Long alignment:"
"$APP" align "$DATA_DIR/align_long_input.txt"
echo

echo "Regex match, true:"
"$APP" regex-match "$DATA_DIR/regex_true.txt"
echo

echo "Regex match, false:"
"$APP" regex-match "$DATA_DIR/regex_false.txt"
echo

echo "K-similarity:"
"$APP" k-similarity "$DATA_DIR/k_similarity_input.txt"
echo

echo "Min window:"
"$APP" min-window "$DATA_DIR/min_window_input.txt"
echo

echo "Min window with repeated pattern symbols:"
"$APP" min-window "$DATA_DIR/min_window_repeated_input.txt"
echo