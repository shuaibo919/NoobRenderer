set(TARGET_NAME Third)

# GLAD
set(GLAD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glad)
file(GLOB GLAD_SOURCE ${GLAD_SOURCE_DIR}/src/*.c)
add_library(glad STATIC ${GLAD_SOURCE})
target_include_directories(glad PUBLIC $<BUILD_INTERFACE:${GLAD_SOURCE_DIR}/include>)

# GLFW
add_subdirectory(glfw)

# Imgui
file(GLOB IMGUI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp)
file(GLOB IMGUI_BACKEND_SRC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/*.cpp)
add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_BACKEND_SRC})
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>)
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${GLAD_SOURCE_DIR}/include>)
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glfw/include>)
target_link_libraries(imgui opengl32 glfw)

# Assimp
add_subdirectory(assimp)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/assimp/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/assimp/config.h")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assimp/contrib/zlib/zconf.h.cmakein" "${CMAKE_CURRENT_SOURCE_DIR}/assimp/contrib/zlib/zconf.h")
