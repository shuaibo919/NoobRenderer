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

    if(APPLE)
        # enable_language(OBJCXX)
        file(GLOB PLUTO_GRAPHICS_BACKEND_MM ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Backend/Vulkan/*.mm)
        list(APPEND PLUTO_GRAPHICS_BACKEND ${PLUTO_GRAPHICS_BACKEND_MM})
    endif()

else()
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
    target_link_libraries(${TARGET_NAME} PRIVATE Vulkan::Vulkan)

    if(APPLE)
        target_link_libraries(${TARGET_NAME} PRIVATE "-framework QuartzCore")
    endif()
else()
    add_library(glad_lib STATIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glad/src/glad.c)
    target_include_directories(glad_lib PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/glad/include)
    target_link_libraries(${TARGET_NAME} PRIVATE glad_lib)
endif()

# Dependencies ---------------------------------------
# # Third-Dependencies: GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG "3.4" # 3.4
    GIT_PROGRESS TRUE
    GIT_SHALLOW     TRUE
    CMAKE_ARGS
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
        -DGLFW_INSTALL=OFF
        -Wno-dev 
        -Wno-deprecated
)
FetchContent_MakeAvailable(glfw)

# # Third-Dependencies: GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG "18feaec455f8ee1f256d91f515fb02d66268ba14" # bump to cmake 3.10
    GIT_PROGRESS TRUE
    GIT_SHALLOW  TRUE
    CMAKE_ARGS
        -Wno-dev 
)
FetchContent_GetProperties(glm)
FetchContent_MakeAvailable(glm)
target_include_directories(${TARGET_NAME} PUBLIC ${glm_SOURCE_DIR}/)

# # Third-Dependencies: VMA
if(${BUILD_WITH_VULKAN_BACKEND})
    FetchContent_Declare(
        vma
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
        GIT_TAG "v3.1.0" # 3.10
        GIT_PROGRESS TRUE
        CMAKE_ARGS
            -DVMA_BUILD_SAMPLES=OFF
            -DVMA_BUILD_DOCUMENTATION=OFF
            -Wno-dev 
            -Wno-deprecated
    )
    FetchContent_MakeAvailable(vma)
    target_link_libraries(${TARGET_NAME} PRIVATE GPUOpen::VulkanMemoryAllocator)
endif()


# # Third-Dependencies: Source Code or Header Integration
target_include_directories(${TARGET_NAME} PUBLIC ${PLUTO_ENGINE_THIRD_LIB_PATH}/json)
target_include_directories(${TARGET_NAME} PRIVATE ${PLUTO_ENGINE_THIRD_LIB_PATH}/stb_image/include)

# # Third-Dependencies: Lib Linking
target_link_libraries(${TARGET_NAME} PRIVATE glfw glm::glm)

# Shader Preprocess ---------------------------------------
add_subdirectory(ShaderFiles)