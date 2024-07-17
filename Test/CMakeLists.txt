set(TARGET_NAME Test)
message(STATUS "Generate Test File Enabled")



file(GLOB TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/RHI/OpenGL/*.cpp)

foreach(_var ${TEST_SRC})
    get_filename_component(app_name ${_var} NAME_WE)
    add_executable(${app_name} ${_var})
    target_link_libraries(${app_name} PUBLIC ${PLUTO_ENGINE_NAME})
    target_include_directories(${app_name} PUBLIC ${CMAKE_BEGIN_PATH}/Pluto)
    add_dependencies(${app_name} ${PLUTO_ENGINE_NAME})
    message(STATUS "Test ${app_name} Generated!")
endforeach()
