# Global Project Configuration
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
project(Pluto)

# Build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(GENERATE_TESTS ON)
    add_definitions(-DPLUTO_DEBUG)
else()
    SET(GENERATE_TESTS OFF)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"Libcmtd.lib")
    set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"Libcmt.lib")
endif()

# Include Guards
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug)

set(PLUTO_ENGINE_NAME PlutoEngine)
set(CMAKE_BEGIN_PATH ${${CMAKE_CURRENT_SOURCE_DIR}})

add_subdirectory(Pluto)
add_subdirectory(PlutoEditor)

if(${GENERATE_TESTS})
    add_subdirectory(Test)
endif()
