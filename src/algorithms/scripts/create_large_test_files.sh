#!/usr/bin/env bash

set -e

DATA_DIR="test_data_large"

mkdir -p "$DATA_DIR"

echo "Creating large test files in $DATA_DIR/"

python3 <<'PY'
from pathlib import Path

DATA_DIR = Path("test_data_large")
DATA_DIR.mkdir(exist_ok=True)

MAX_DNA_LEN = 10_000

def write_file(path: Path, content: str) -> None:
    path.write_text(content, encoding="utf-8")

def dna_repeat(pattern: str, length: int) -> str:
    return (pattern * ((length // len(pattern)) + 1))[:length]

exact_text = dna_repeat("ACGTGGTACCGTTAAC", MAX_DNA_LEN)
exact_pattern = "GGTACCGT"

write_file(DATA_DIR / "exact_large_text.txt", exact_text + "\n")
write_file(DATA_DIR / "exact_large_pattern.txt", exact_pattern + "\n")

exact_no_match_text = "A" * MAX_DNA_LEN
exact_no_match_pattern = "CGTCGTCGT"

write_file(DATA_DIR / "exact_large_no_match_text.txt", exact_no_match_text + "\n")
write_file(DATA_DIR / "exact_large_no_match_pattern.txt", exact_no_match_pattern + "\n")

align_len = 1000

align_a = dna_repeat("ACGTACGTGGTT", align_len)
align_b = dna_repeat("ACGTTCGTAGTT", align_len)

write_file(
    DATA_DIR / "align_large_input.txt",
    f"2 -1 -2\n{align_a}\n{align_b}\n"
)

write_file(
    DATA_DIR / "align_score_large_input.txt",
    f"2 -1 -2\n{align_a}\n{align_b}\n"
)

regex_text = dna_repeat("ACGT", MAX_DNA_LEN)
regex_pattern_true = "A.*T"
regex_pattern_false = "TAAAACCCC"

write_file(DATA_DIR / "regex_large_true.txt", f"{regex_text}\n{regex_pattern_true}\n")
write_file(DATA_DIR / "regex_large_false.txt", f"{regex_text}\n{regex_pattern_false}\n")

k_a = "AACCGGTTAACC"
k_b = "TTCCAACCGGAA"

write_file(DATA_DIR / "k_similarity_large_input.txt", f"{k_a}\n{k_b}\n")

prefix = "A" * 4500
middle = "GGGTTTCCCAAAGCTA"
suffix = "C" * (MAX_DNA_LEN - len(prefix) - len(middle))

min_window_text = prefix + middle + suffix
min_window_pattern = "GCTA"

write_file(
    DATA_DIR / "min_window_large_input.txt",
    f"{min_window_text}\n{min_window_pattern}\n"
)

prefix = "G" * 4500
middle = "TTTAAACCCGGGAAACC"
suffix = "T" * (MAX_DNA_LEN - len(prefix) - len(middle))

min_window_repeated_text = prefix + middle + suffix
min_window_repeated_pattern = "AACC"

write_file(
    DATA_DIR / "min_window_large_repeated_input.txt",
    f"{min_window_repeated_text}\n{min_window_repeated_pattern}\n"
)

print("Large test files created successfully.")
PY

echo "Done."