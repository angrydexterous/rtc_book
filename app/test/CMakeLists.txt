find_package(GTest REQUIRED)
add_test(NAME rtc_project_tests COMMAND rtc_project_tests)
set_tests_properties(${conan_project_tests} PROPERTIES TIMEOUT 10)

set(SOURCE_FILES main.cpp 
                 app/tuple_tests.cpp
                 app/color_tests.cpp
                 app/canvas_tests.cpp
                 app/matrix_tests.cpp
)
add_executable(rtc_project_tests ${SOURCE_FILES})
target_include_directories(rtc_project_tests PRIVATE ${app_SOURCE_DIR}/include)
target_link_libraries(rtc_project_tests rtc_project_as_lib 
                                        GTest::gtest_main
                                        )