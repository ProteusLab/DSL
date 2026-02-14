#!/usr/bin/env bash
set -euo pipefail

PRESET_NAME=$1

# Configure CMake
cmake -S . \
  --preset "${PRESET_NAME}" -G Ninja \
  -D CT_TREAT_WARNINGS_AS_ERRORS=ON

# Build
cmake --build "build/${PRESET_NAME}" -j
