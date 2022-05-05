# Add Deps Here
find_package(spdlog CONFIG)
find_package(fmt REQUIRED)
find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)
find_package(Eigen3 REQUIRED CONFIG)

set(app_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(RUN_TESTS)
    add_subdirectory(test)
endif()

#  MAIN LIBRARY
set(SOURCE_FILES_AS_LIBS src/canvas.cpp
)

# SETUP LIBRARIES FOR LINK
set(TARGET_LIBS spdlog::spdlog
                fmt::fmt 
                imgui::imgui         
                glfw
                GLEW::glew_s
                GTest::gtest_main
                Eigen3::Eigen
)


add_library(rtc_project_as_lib ${SOURCE_FILES_AS_LIBS} ${INCLUDE_FILES})
target_include_directories(rtc_project_as_lib PRIVATE
        include
        )
target_link_libraries(rtc_project_as_lib ${TARGET_LIBS})

# MAIN EXECUTABLE
set(SOURCE_FILES src/main.cpp
                 include/imgui-bindings/imgui_impl_glfw.cpp
                 include/imgui-bindings/imgui_impl_opengl3.cpp
                 )

set(INCLUDE_FILES include/imgui-bindings/imgui_impl_glfw.h
                  include/imgui-bindings/imgui_impl_opengl3.h
)
                 
add_executable(rtc_project ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(rtc_project PRIVATE
        include
        )

target_compile_definitions(rtc_project PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(rtc_project rtc_project_as_lib ${TARGET_LIBS})

