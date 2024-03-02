message("Generate Test File Enabled")

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})

foreach(_var ${SRC})
    get_filename_component(app_name ${_var} NAME_WE)
    add_executable(${app_name} ${_var})
    add_dependencies(${app_name} imgui Engine)
    target_link_libraries(${app_name} PUBLIC imgui Engine)
    target_include_directories(${app_name} PUBLIC ${ROOT_DIR}/)
    target_include_directories(${app_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
    target_include_directories(${app_name} PUBLIC ${ROOT_DIR}/Third/imgui)
    target_include_directories(${app_name} PUBLIC ${ROOT_DIR}/Third/imgui/backends)
    message("Test ${app_name} Generated!")
endforeach()
