#!/usr/bin/env bash
set -e  # stop on first error

BUILD_DIR=build
EXECUTABLE=A1_2

# Configure
cmake -S . -B "$BUILD_DIR"

# Build
cmake --build "$BUILD_DIR"

# Run
"./$BUILD_DIR/$EXECUTABLE"

#Run plotting
#python3 plot_results.py 