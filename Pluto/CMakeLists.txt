set(TARGET_NAME ${PLUTO_ENGINE_NAME})
include(FetchContent)
project(${TARGET_NAME})

# Pluto Engine ---------------------------------------

# # Pluto Engine : Core/ - Source
file(GLOB PLUTO_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.cpp)
file(GLOB PLUTO_CORE_UTILITY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/Utilities/*.cpp)

# # Pluto Engine : Graphics/Backend/ - Source
if(${BUILD_WITH_VULKAN_BACKEND})
    message(STATUS "Build with Vulkan Backend")
    add_definitions(-DVULKAN_BACKEND)
    file(GLOB PLUTO_GRAPHICS_BACKEND ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Backend/Vulkan/*.cpp)
elseif()
    message(STATUS "Build with OpenGL Backend")
    add_definitions(-DOPENGL_BACKEND)
    file(GLOB PLUTO_GRAPHICS_BACKEND ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Backend/OpenGL/*.cpp)
endif()

# # Pluto Engine : Graphics/ - Source
file(GLOB PLUTO_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/*.cpp)
file(GLOB PLUTO_GRAPHICS_RHI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/RHI/*.cpp)

# # Pluto Engine : Third/ - Source
set(PLUTO_ENGINE_THIRD_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Third)

# # Pluto Engine : Build
add_library(${TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/PlutoEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PlutoEngine.h
    ${PLUTO_CORE_SRC}
    ${PLUTO_CORE_UTILITY_SRC}
    ${PLUTO_GRAPHICS_SRC}
    ${PLUTO_GRAPHICS_RHI_SRC}
    ${PLUTO_GRAPHICS_BACKEND}
)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# # Pluto Engine : Graphics Api
if(${BUILD_WITH_VULKAN_BACKEND})
    find_package(Vulkan REQUIRED)
    target_link_libraries(${TARGET_NAME} Vulkan::Vulkan)
elseif()
    add_library(glad_lib STATIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glad/src/glad.c)
    target_include_directories(glad_lib PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glad/include)
    target_link_libraries(${TARGET_NAME} glad_lib)
endif()

# Dependencies ---------------------------------------
# # Third-Dependencies: GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG "b35641f4a3c62aa86a0b3c983d163bc0fe36026d" # 3.4
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

# # Third-Dependencies: GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/icaven/glm
    GIT_TAG "dca38025fba63bb9284023e6de55f756b9e37cec"
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    target_include_directories(${TARGET_NAME} PUBLIC ${glm_SOURCE_DIR}/)
endif()

# # Third-Dependencies: VMA
if(${BUILD_WITH_VULKAN_BACKEND})
    FetchContent_Declare(
        vma
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
        GIT_TAG "009ecd192c1289c7529bff248a16cfe896254816" # 3.10
        GIT_PROGRESS TRUE
    )
    FetchContent_GetProperties(vma)

    if(NOT vma_POPULATED)
        FetchContent_Populate(vma)
        target_include_directories(${TARGET_NAME} PRIVATE ${vma_SOURCE_DIR}/include)
    endif()
endif()

# # Third-Dependencies: Source Code or Header Integration
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/json)
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/stb_image/include)

# # Third-Dependencies: Lib Linking
target_link_libraries(${TARGET_NAME} glfw)

# Shader Preprocess ---------------------------------------
add_subdirectory(ShaderFiles)