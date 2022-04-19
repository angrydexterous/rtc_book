# Add Deps Here
find_package(spdlog)
find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)

set(app_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(RUN_TESTS)
    add_subdirectory(test)
endif()

set(SOURCE_FILES src/main.cpp
                 include/imgui-bindings/imgui_impl_glfw.cpp
                 include/imgui-bindings/imgui_impl_opengl3.cpp
                 )

set(INCLUDE_FILES  include/imgui-bindings/imgui_impl_glfw.h
                   include/imgui-bindings/imgui_impl_opengl3.h
)
                 
add_executable(rtc_project ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(rtc_project PRIVATE
        include
        ${spdlog_INCLUDE_DIR}
        )
target_compile_definitions(rtc_project PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(rtc_project spdlog::spdlog 
                                  imgui::imgui         
                                  glfw
                                  GLEW::glew_s)