#!/bin/bash

# check parameters
if [ $# -eq 0 ]; then
    echo "[Error]: No backend specified!"
    echo "[Help Message / Parameters]: sh $0 [backend](opengl/vulkan)"
    echo "[Help Message / Example]:    sh $0  vulkan"
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
        echo "Unsupported backend '$1'. Supported backends are 'vulkan' and 'opengl'."
        exit 1
        ;;
esac

cmake -S . -B "$BUILD_DIR" -DBUILD_WITH_VULKAN_BACKEND="$VULKAN_BACKEND"
cmake --build "$BUILD_DIR" -j32
