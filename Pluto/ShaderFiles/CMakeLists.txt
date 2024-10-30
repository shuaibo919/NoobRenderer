set(TARGET_NAME ShaderPreprocessing)
project(${TARGET_NAME})

add_executable(${TARGET_NAME} Preprocess.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_BINARY_DIR}/Include)
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/json)
target_link_libraries(${TARGET_NAME} PRIVATE spirv-cross-glsl ${PLUTO_ENGINE_NAME})

# Find glslc
find_package(Vulkan COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

# Third-Lib: SPIRV-Cross
FetchContent_Declare(
    spirvcross
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross
    GIT_TAG "5d127b917f080c6f052553c47170ec0ba702e54f" # "vulkan-sdk-1.3.290"
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(spirvcross)

if(NOT spirvcross_POPULATED)
    FetchContent_Populate(spirvcross)
    option(SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS "Instead of throwing exceptions assert" OFF)
    option(SPIRV_CROSS_SHARED "Build the C API as a single shared library." OFF)
    option(SPIRV_CROSS_STATIC "Build the C and C++ API as static libraries." ON)
    option(SPIRV_CROSS_CLI "Build the CLI binary. Requires SPIRV_CROSS_STATIC." OFF)
    option(SPIRV_CROSS_ENABLE_TESTS "Enable SPIRV-Cross tests." OFF)
    add_subdirectory(${spirvcross_SOURCE_DIR} ${spirvcross_BINARY_DIR})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(EXECUTABLE bin/${CMAKE_BUILD_TYPE}/${TARGET_NAME}.exe)
else()
    set(EXECUTABLE bin/${CMAKE_BUILD_TYPE}/${TARGET_NAME})
endif()

add_custom_target(GenerateShader ALL DEPENDS ${TARGET_NAME} ${PLUTO_ENGINE_NAME} ${PlutoEditor})

# generate shader files
add_custom_command(
    TARGET GenerateShader
    COMMAND ${EXECUTABLE} Pluto/ShaderFiles/ bin/${EXECUTABLE_OUTPUT_PREFIX}/${CMAKE_BUILD_TYPE}/Asset/Shader/ ${glslc_executable}
    WORKING_DIRECTORY ${CMAKE_BEGIN_PATH}
)

add_dependencies(${TARGET_NAME} ${PLUTO_ENGINE_NAME})
