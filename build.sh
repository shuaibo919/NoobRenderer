#!/bin/bash

# check parameters
if [ $# -eq 0 ]; then
    echo "[build.sh][Error]: No backend specified!"
    echo "[build.sh][Help Message / Parameters]: sh $0 [backend](opengl/vulkan)"
    echo "[build.sh][Help Message / Example]:    sh $0  vulkan"
    exit 1
fi

# choose backend
case "$1" in
    vulkan)
        VULKAN_BACKEND="ON"
        BUILD_DIR="build-vk"
        ;;
    opengl)
        VULKAN_BACKEND="OFF"
        BUILD_DIR="build-gl"
        ;;
    *)
        echo "[build.sh]:Unsupported backend '$1'. Supported backends are 'vulkan' and 'opengl'."
        exit 1
        ;;
esac

# check for -j parameter 
# example : sh build.sh vulkan -p 64
PARALLELISM=32
for arg in "$@"; do
    if [ "$arg" == "-p" ]; then
        shift
        PARALLELISM=$2
        shift
        echo "[build.sh]:CMake using $PARALLELISM parallel jobs to build."
        break
    fi
    echo "[build.sh]:Default $PARALLELISM parallel jobs equal to 32, using -p [number] to change it."
done


cmake -S . -B "$BUILD_DIR" -DBUILD_WITH_VULKAN_BACKEND="$VULKAN_BACKEND"
cmake --build "$BUILD_DIR" -j"$PARALLELISM"
