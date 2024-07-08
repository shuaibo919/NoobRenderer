set(TARGET_NAME ${PLUTO_ENGINE_NAME})

add_definitions(-DOPENGL_BACKEND)

# Core-Layer-Source
file(GLOB_RECURSE PLUTO_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.cpp)
file(GLOB_RECURSE PLUTO_CORE_UTILITY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/Utilities/*.cpp)

# Graphics-Layer-Source
file(GLOB_RECURSE PLUTO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/*.cpp)
file(GLOB_RECURSE PLUTO_GRAPHICS_RHI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/RHI*.cpp)
file(GLOB_RECURSE PLUTO_GRAPHICS_OPENGL ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Backend/OpenGL/*.cpp)
file(GLOB_RECURSE PLUTO_GRAPHICS_VULKAN ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Backend/Vulkan/*.cpp)

# Graphics-Layer
set(PLUTO_ENGINE_GRAPHICS Graphics)

project(${TARGET_NAME})

set(PLUTO_ENGINE_THIRD_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Third)

add_library(${TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/PlutoEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PlutoEngine.h
    ${PLUTO_CORE_SRC}
    ${PLUTO_CORE_UTILITY_SRC}
    ${PLUTO_GRAPHICS_SRC}
    ${PLUTO_GRAPHICS_RHI_SRC}
    ${PLUTO_GRAPHICS_OPENGL}
    ${PLUTO_GRAPHICS_VULKAN}
)

# Third-Lib-glfw
add_subdirectory(Third/glfw)

# Third-Lib-SpirvCross
add_subdirectory(Third/SPIRV-Cross)

# Third-Lib-glad
add_library(glad_lib STATIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glad/src/glad.c)
target_include_directories(glad_lib PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glad/include)

# Third-Lib-Headers
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glm)
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glad/include)
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glfw/include)
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/SPIRV-Cross/include)
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/stb_image/include)

# Core-Layer-Headers
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} glfw glad_lib spirv-cross-reflect)