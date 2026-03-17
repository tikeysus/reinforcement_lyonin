#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
OUTPUT_FILE="$ROOT_DIR/experiment_results.csv"

if [[ ! -x "$BUILD_DIR/mc" || ! -x "$BUILD_DIR/sarsa" || ! -x "$BUILD_DIR/q" || ! -x "$BUILD_DIR/expected" || ! -x "$BUILD_DIR/doubleq" ]]; then
    echo "Missing executables. Build the project first with:"
    echo "  cmake -S . -B build"
    echo "  cmake --build build"
    exit 1
fi

printf 'algorithm,epsilon_mode,alpha,epsilon,episodes_per_run,runs,overall_episodes,total_steps,total_time_ms,avg_norm_steps,avg_norm_time_ms\n' > "$OUTPUT_FILE"

for algo in mc sarsa q expected doubleq; do
    for mode in fixed linear; do
        for alpha in 0.05 0.1 0.2; do
            "$BUILD_DIR/$algo" \
                --p1 1 \
                --p2 0 \
                --episodes 25000 \
                --alpha "$alpha" \
                --epsilon 0.1 \
                --epsilon-mode "$mode" \
                --runs 20 \
                --csv >> "$OUTPUT_FILE"
        done
    done
done

echo "Saved results to $OUTPUT_FILE"
