#!/bin/bash

set -e
if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: $0 <test_dir> <source_dir>"
    exit 1
fi

THRESHOLD=80
TEST_DIR="$1"
SOURCE_DIR="$2"
BINARIES=$TEST_DIR/test_*
HTML_OUTPUT="$TEST_DIR/coverage-html"
PROFDATA="$TEST_DIR/coverage.profdata"

echo "Binaries is: " ${BINARIES[@]}

# 1. Merge all .profraw files into a single .profdata
echo "Merging profile data..."
llvm-profdata merge -sparse "$TEST_DIR"/*.profraw -o "$PROFDATA"

# 2. Generate HTML coverage report
echo "Generating HTML report..."
llvm-cov show ${BINARIES[@]} \
    -instr-profile="$PROFDATA" \
    -format=html \
    -output-dir="$HTML_OUTPUT" \
    $SOURCE_DIR
echo "HTML report generated in $HTML_OUTPUT"

# 3. Get total branch coverage
COVERAGE=$(llvm-cov report ${BINARIES[@]} -instr-profile="$PROFDATA" $SOURCE_DIR | \
           awk '/^TOTAL/ {gsub("%","",$13); print $13}')  # 13th column = branch coverage

# 4. Fail if coverage is below threshold
if (( $(echo "$COVERAGE < $THRESHOLD" | bc -l) )); then
    echo "ERROR: Branch coverage $COVERAGE% is below threshold $THRESHOLD%"
    exit 1
fi

echo "Branch coverage $COVERAGE% meets threshold ✅"
