set(TARGET_NAME PlutoEditor)
project(${TARGET_NAME})

set(EDITOR_SOURCES
    Main.cpp
)

add_executable(${TARGET_NAME} ${EDITOR_SOURCES})

target_link_libraries(${TARGET_NAME}
    ${PLUTO_ENGINE_NAME}
)