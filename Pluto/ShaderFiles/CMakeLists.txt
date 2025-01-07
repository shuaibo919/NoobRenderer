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
    GIT_TAG "104d91a9c386b7e4b60463e1b3c51aa353c4c4bc"
    # URL_HASH SHA256=82331a2ca9f2ea90d05901e931aa8c21e70dc40c # "bump to cmake 3.10"
    GIT_PROGRESS TRUE
    GIT_SHALLOW  TRUE
    CMAKE_ARGS
        -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS=OFF
        -DSPIRV_CROSS_SHARED=OFF
        -DSPIRV_CROSS_STATIC=ON
        -DSPIRV_CROSS_CLI=OFF
        -DSPIRV_CROSS_ENABLE_TESTS=OFF
        -Wno-dev 
)

FetchContent_GetProperties(spirvcross)
FetchContent_MakeAvailable(spirvcross)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(EXECUTABLE bin/${EXECUTABLE_OUTPUT_PREFIX}/${CMAKE_BUILD_TYPE}/${TARGET_NAME}.exe)
else()
    set(EXECUTABLE bin/${EXECUTABLE_OUTPUT_PREFIX}/${CMAKE_BUILD_TYPE}/${TARGET_NAME})
endif()

add_custom_target(GenerateShader ALL DEPENDS ${TARGET_NAME} ${PLUTO_ENGINE_NAME} ${PlutoEditor})

# generate shader files
add_custom_command(
    TARGET GenerateShader POST_BUILD
    COMMAND ${EXECUTABLE} Pluto/ShaderFiles/ bin/${EXECUTABLE_OUTPUT_PREFIX}/${CMAKE_BUILD_TYPE}/Asset/Shader/ ${glslc_executable}
    WORKING_DIRECTORY ${CMAKE_BEGIN_PATH}
)

add_dependencies(${TARGET_NAME} ${PLUTO_ENGINE_NAME})
