#!/bin/bash

BUILD_TYPE="debug"
SRC_DIR=$(dirname "${BASH_SOURCE[0]}")

if [[ "$1" == "--release" ]]; then
    BUILD_TYPE="release"
fi

mkdir -p "${SRC_DIR}/${BUILD_TYPE}-build"
cmake -S "${SRC_DIR}" -B "${SRC_DIR}/${BUILD_TYPE}-build" --preset "gcc-${BUILD_TYPE}"
cmake --build "${SRC_DIR}/${BUILD_TYPE}-build"

ln -sf "${SRC_DIR}/${BUILD_TYPE}/compile_commands.json" "${SRC_DIR}/compile_commands.json"
