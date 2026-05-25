#!/usr/bin/env bash

set -e

APP="../build/dna/dna_runner"
DATA_DIR="test_data_large"

MAX_TIME_SECONDS=1
MAX_MEMORY_KB=131072 # 128 MB

if [ ! -x "$APP" ]; then
  echo "Application binary not found: $APP"
  echo "Run: make app"
  exit 1
fi

if [ ! -d "$DATA_DIR" ]; then
  echo "Large test data directory not found: $DATA_DIR"
  echo "Run: ./scripts/create_large_test_files.sh"
  exit 1
fi

if command -v gtime >/dev/null 2>&1; then
  TIME_CMD="gtime"
elif /usr/bin/time -v true >/dev/null 2>&1; then
  TIME_CMD="/usr/bin/time"
else
  echo "GNU time is required."
  echo "On macOS install it with:"
  echo "  brew install gnu-time"
  exit 1
fi

elapsed_to_seconds() {
  python3 - "$1" <<'PY'
import sys

value = sys.argv[1].strip()

# GNU time formats elapsed time like:
# 0:00.12
# 1:02.34
# 1:02:03
parts = value.split(":")

if len(parts) == 3:
    hours = int(parts[0])
    minutes = int(parts[1])
    seconds = float(parts[2])
    print(hours * 3600 + minutes * 60 + seconds)
elif len(parts) == 2:
    minutes = int(parts[0])
    seconds = float(parts[1])
    print(minutes * 60 + seconds)
else:
    print(float(value))
PY
}

check_time_limit() {
  python3 - "$1" "$2" <<'PY'
import sys

elapsed = float(sys.argv[1])
limit = float(sys.argv[2])

if elapsed > limit:
    print(f"FAILED: elapsed time {elapsed:.4f}s exceeds limit {limit:.4f}s")
    sys.exit(1)
PY
}

run_check() {
  local name="$1"
  shift

  local report_file
  report_file="$(mktemp)"

  echo "Checking: $name"

  set +e
  "$TIME_CMD" -v "$@" >/dev/null 2>"$report_file"
  local exit_code=$?
  set -e

  if [ "$exit_code" -ne 0 ]; then
    echo "FAILED: command exited with status $exit_code"
    echo
    cat "$report_file"
    rm -f "$report_file"
    exit 1
  fi

  local elapsed_raw
  local memory_kb
  local elapsed_seconds

  elapsed_raw="$(grep "Elapsed (wall clock) time" "$report_file" | awk -F': ' '{print $2}')"
  memory_kb="$(grep "Maximum resident set size" "$report_file" | awk -F': ' '{print $2}')"

  if [ -z "$elapsed_raw" ]; then
    echo "FAILED: could not read elapsed time from time output"
    cat "$report_file"
    rm -f "$report_file"
    exit 1
  fi

  if [ -z "$memory_kb" ]; then
    echo "FAILED: could not read memory usage from time output"
    cat "$report_file"
    rm -f "$report_file"
    exit 1
  fi

  elapsed_seconds="$(elapsed_to_seconds "$elapsed_raw")"

  if ! check_time_limit "$elapsed_seconds" "$MAX_TIME_SECONDS"; then
    rm -f "$report_file"
    exit 1
  fi

  if [ "$memory_kb" -gt "$MAX_MEMORY_KB" ]; then
    echo "FAILED: memory ${memory_kb} KB exceeds limit ${MAX_MEMORY_KB} KB"
    rm -f "$report_file"
    exit 1
  fi

  echo "OK: time=${elapsed_seconds}s memory=${memory_kb}KB"
  echo

  rm -f "$report_file"
}

run_check \
  "exact search large" \
  "$APP" exact-search "$DATA_DIR/exact_large_text.txt" "$DATA_DIR/exact_large_pattern.txt"

run_check \
  "exact search large no match" \
  "$APP" exact-search "$DATA_DIR/exact_large_no_match_text.txt" "$DATA_DIR/exact_large_no_match_pattern.txt"

run_check \
  "align score large" \
  "$APP" align-score "$DATA_DIR/align_score_large_input.txt"

run_check \
  "full alignment large" \
  "$APP" align "$DATA_DIR/align_large_input.txt"

run_check \
  "regex match large true" \
  "$APP" regex-match "$DATA_DIR/regex_large_true.txt"

run_check \
  "regex match large false" \
  "$APP" regex-match "$DATA_DIR/regex_large_false.txt"

run_check \
  "k-similarity large" \
  "$APP" k-similarity "$DATA_DIR/k_similarity_large_input.txt"

run_check \
  "min window large" \
  "$APP" min-window "$DATA_DIR/min_window_large_input.txt"

run_check \
  "min window large repeated" \
  "$APP" min-window "$DATA_DIR/min_window_large_repeated_input.txt"

echo "All performance checks passed."