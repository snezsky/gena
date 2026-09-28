#!/bin/bash

set -e
if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: $0 <build_dir> <source_dir>..."
    exit 1
fi

THRESHOLD=80
BUILD_DIR="$1"
shift
HTML_OUTPUT="$BUILD_DIR/coverage-html"
COBERTURA="$BUILD_DIR/coverage.xml"

FILTERS=()
for SOURCE_DIR in "$@"; do
    FILTERS+=(--filter "$SOURCE_DIR/")
done

# Needs a build compiled with --coverage and the tests already run, set GCOV="llvm-cov gcov" for clang builds.
# Throw and unreachable branches are excluded, C++ branch coverage can't reach the threshold otherwise.
mkdir -p "$HTML_OUTPUT"
gcovr --root . "$BUILD_DIR" \
    --gcov-executable "${GCOV:-gcov}" \
    --exclude-directories "$BUILD_DIR/deps" \
    "${FILTERS[@]}" \
    --exclude-throw-branches \
    --exclude-unreachable-branches \
    --html-details "$HTML_OUTPUT/index.html" \
    --cobertura "$COBERTURA" \
    --print-summary \
    --fail-under-branch $THRESHOLD
