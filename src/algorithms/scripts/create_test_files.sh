#!/usr/bin/env bash

set -e

DATA_DIR="test_data"

mkdir -p "$DATA_DIR"

# Exact search:
# Pattern "ACGTAC" встречается несколько раз в тексте.
cat > "$DATA_DIR/exact_text.txt" <<EOF
TTACGTACGGGACGTACCCACGTAC
EOF

cat > "$DATA_DIR/exact_pattern.txt" <<EOF
ACGTAC
EOF

# Exact search, no matches:
cat > "$DATA_DIR/exact_no_match_text.txt" <<EOF
AAAACCCCGGGGTTTTAAAACCCC
EOF

cat > "$DATA_DIR/exact_no_match_pattern.txt" <<EOF
TGCATGCA
EOF

# Alignment score / full alignment:
# match = 2, mismatch = -1, gap = -2
cat > "$DATA_DIR/align_input.txt" <<EOF
2 -1 -2
GATTACA
GCATGCA
EOF

cat > "$DATA_DIR/align_score_input.txt" <<EOF
2 -1 -2
GATTACA
GCATGCA
EOF

# Более длинный alignment input
cat > "$DATA_DIR/align_long_input.txt" <<EOF
2 -1 -2
ACCGTGAACCTG
ACGTCGACCTGA
EOF

# Regex match true:
# Regex "A.*CGT.*TTA" должен матчить строку.
# В тексте только A/C/G/T, а служебные символы regex остаются в паттерне.
cat > "$DATA_DIR/regex_true.txt" <<EOF
AAACCCGGGTTTACGTAATTA
A.*CGT.*TTA
EOF

# Regex match false:
cat > "$DATA_DIR/regex_false.txt" <<EOF
AAACCCGGGTTTACGTAATTA
A.*GGG.*CCCC
EOF

# K-similarity:
# Строки являются анаграммами, состоят только из A/C/G/T.
cat > "$DATA_DIR/k_similarity_input.txt" <<EOF
AACCGTTG
TTCCAAGG
EOF

# Min window:
# Нужно найти минимальное окно, содержащее все символы паттерна.
cat > "$DATA_DIR/min_window_input.txt" <<EOF
TTAACCGGTTAACGGTACCTAGGCTA
GCTA
EOF

# Min window, пример с повторяющимися символами в паттерне.
cat > "$DATA_DIR/min_window_repeated_input.txt" <<EOF
AAACGTTACGGAACCTTGGCA
AACC
EOF

echo "Test files created in $DATA_DIR/"