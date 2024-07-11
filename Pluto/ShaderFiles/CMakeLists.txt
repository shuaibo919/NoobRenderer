set(TARGET_NAME ShaderPreprocessing)
project(${TARGET_NAME})

add_executable(${TARGET_NAME} Preprocess.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/json)
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/SPIRV-Cross/)
target_link_libraries(${TARGET_NAME} PRIVATE spirv-cross-core spirv-cross-glsl)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(EXECUTABLE bin/${CMAKE_BUILD_TYPE}/${TARGET_NAME}.exe)
else()
    set(EXECUTABLE bin/${CMAKE_BUILD_TYPE}/${TARGET_NAME})
endif()

add_custom_target(GenerateShader ALL DEPENDS ${TARGET_NAME} ${PLUTO_ENGINE_NAME} ${PlutoEditor})

add_custom_command(
    TARGET GenerateShader
    COMMAND ${EXECUTABLE} Pluto/ShaderFiles/ bin/${CMAKE_BUILD_TYPE}/Asset/Shader/
    WORKING_DIRECTORY ${CMAKE_BEGIN_PATH}
)

add_dependencies(${TARGET_NAME} ${PLUTO_ENGINE_NAME})