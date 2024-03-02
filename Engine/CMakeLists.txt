set(TARGET_NAME Engine)
project(${TARGET_NAME})

configure_file(EngineConfig.json ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/EngineConfig.json)

# Engine
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Core
file(GLOB CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.cpp)

# Function
file(GLOB FUNCTION_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Function/*.cpp)
file(GLOB FUNCTION_BUFFER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Function/Buffer/*.cpp)
file(GLOB FUNCTION_COMPONENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Function/Component/*.cpp)
file(GLOB FUNCTION_GRAPHICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Function/Graphics/*.cpp)
file(GLOB FUNCTION_GRAPHICS_MATERIAL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Function/Graphics/Material/*.cpp)
file(GLOB FUNCTION_GRAPHICS_TEXTURE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Function/Graphics/Texture/*.cpp)
file(GLOB FUNCTION_UTILITY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Function/Utility/*.cpp)

# Render
file(GLOB RENDER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Render/*.cpp)
file(GLOB RENDER_COLLECTIONS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Render/Collections/*.cpp)
file(GLOB RENDER_PASS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Render/Pass/*.cpp)

# System
file(GLOB SYSTEM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/System/*.cpp)

list(APPEND ENGINE_SOURCE
    ${CORE_SRC}
    ${FUNCTION_SRC}
    ${FUNCTION_BUFFER_SRC}
    ${FUNCTION_COMPONENT_SRC}
    ${FUNCTION_GRAPHICS_SRC}
    ${FUNCTION_GRAPHICS_MATERIAL_SRC}
    ${FUNCTION_GRAPHICS_TEXTURE_SRC}
    ${FUNCTION_UTILITY_SRC}
    ${SYSTEM_SRC}
    ${RENDER_SRC}
    ${RENDER_COLLECTIONS_SRC}
    ${RENDER_PASS_SRC}
    ${SRC}
)

# Compile file
add_library(${TARGET_NAME} STATIC ${ENGINE_SOURCE})

# GLAD GLFW
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/)
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/Third/glad/include)
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/Third/glfw/include)
target_link_libraries(${TARGET_NAME} opengl32 glad glfw)

# Header-Only GLM
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/Third/glm)

# Header-Only STB_IMAGE
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/Third/stb_image/include)

# Header-Only nlohmann Json
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/Third/nlohmann/)

# Assimp
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/Third/assimp/include)
target_link_libraries(${TARGET_NAME} assimp)

# Entt
target_link_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/Third/entt/)