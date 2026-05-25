#!/usr/bin/env bash

set -e

DATA_DIR="test_data"

mkdir -p "$DATA_DIR"

cat > "$DATA_DIR/exact_text.txt" <<EOF
TTACGTACGGGACGTACCCACGTAC
EOF

cat > "$DATA_DIR/exact_pattern.txt" <<EOF
ACGTAC
EOF

cat > "$DATA_DIR/exact_no_match_text.txt" <<EOF
AAAACCCCGGGGTTTTAAAACCCC
EOF

cat > "$DATA_DIR/exact_no_match_pattern.txt" <<EOF
TGCATGCA
EOF

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

cat > "$DATA_DIR/align_long_input.txt" <<EOF
2 -1 -2
ACCGTGAACCTG
ACGTCGACCTGA
EOF

cat > "$DATA_DIR/regex_true.txt" <<EOF
AAACCCGGGTTTACGTAATTA
A.*CGT.*TTA
EOF

cat > "$DATA_DIR/regex_false.txt" <<EOF
AAACCCGGGTTTACGTAATTA
A.*GGG.*CCCC
EOF

cat > "$DATA_DIR/k_similarity_input.txt" <<EOF
AACCGTTG
TTCCAAGG
EOF

cat > "$DATA_DIR/min_window_input.txt" <<EOF
TTAACCGGTTAACGGTACCTAGGCTA
GCTA
EOF

cat > "$DATA_DIR/min_window_repeated_input.txt" <<EOF
AAACGTTACGGAACCTTGGCA
AACC
EOF

echo "Test files created in $DATA_DIR/"