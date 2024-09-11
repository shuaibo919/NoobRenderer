set(TARGET_NAME Test)
message(STATUS "Generate Test File Enabled")

# Dev
if(${BUILD_WITH_VULKAN_BACKEND})
    message(STATUS "Generate Vulkan Test Example")
    set(TestSourcePath ${CMAKE_CURRENT_SOURCE_DIR}/RHI/Vulkan/*.cpp)
else()
    message(STATUS "Generate Opengl Test Example")
    set(TestSourcePath ${CMAKE_CURRENT_SOURCE_DIR}/RHI/OpenGL/*.cpp)
endif()

file(GLOB TEST_SRC ${TestSourcePath})

if(${BUILD_WITH_TESTS})
    foreach(_var ${TEST_SRC})
        get_filename_component(app_name ${_var} NAME_WE)
        add_executable(${app_name} ${_var})
        target_link_libraries(${app_name} PUBLIC ${PLUTO_ENGINE_NAME})
        target_include_directories(${app_name} PUBLIC ${CMAKE_BEGIN_PATH}/Pluto)
        add_dependencies(${app_name} ${PLUTO_ENGINE_NAME})
        message(STATUS "Test ${app_name} Generated!")
    endforeach()
endif()