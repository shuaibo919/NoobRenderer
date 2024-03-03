# Global Project Configuration
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
project(InkShadows)

# Generate Test
SET(GENERATE_TESTS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"Libcmtd.lib")
    set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"Libcmt.lib")
else()
    message(FATAL_ERROR "Not yet Support other complier.")
endif()

# Include Guards
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# Set Path
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OUTPUT_DIR ${ROOT_DIR}/bin)
set(THIRD_DIR ${ROOT_DIR}/Third)

add_custom_target(copy_logo ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/Icon/logo.png
    ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/logo.png
)

# copy source
add_custom_target(copy_resource ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ROOT_DIR}/Resource
    ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/Resource
)

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR}/Debug)

# Set Source
add_subdirectory(Third)
add_subdirectory(Engine)
add_subdirectory(Editor)

if(${GENERATE_TESTS})
    add_subdirectory(Test)
endif()
