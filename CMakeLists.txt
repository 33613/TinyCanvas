cmake_minimum_required(VERSION 3.16)
project(TinyCanvas LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CTest)

if(BUILD_TESTING)
    # 每个 lab 独立编译，避免同名头文件被解析到其他实验。
    file(GLOB lab_directories CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/lab_*")
    foreach(lab_directory IN LISTS lab_directories)
        get_filename_component(lab_name "${lab_directory}" NAME)
        if(NOT IS_DIRECTORY "${lab_directory}" OR NOT lab_name MATCHES "^lab_[0-9]+$")
            continue()
        endif()

        # 一个 lab 对应一个测试程序；vendor 中的实现不参与源码收集。
        file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS "${lab_directory}/test/*.cpp")
        list(FILTER test_sources EXCLUDE REGEX "/vendor/")
        if(NOT test_sources)
            continue()
        endif()
        file(GLOB_RECURSE implementation_sources CONFIGURE_DEPENDS "${lab_directory}/src/*.cpp")

        set(doctest_directory "${lab_directory}/test/vendor/doctest")
        if(NOT EXISTS "${doctest_directory}/doctest.h" OR
           NOT EXISTS "${doctest_directory}/doctest.cmake" OR
           NOT EXISTS "${doctest_directory}/doctestAddTests.cmake")
            message(FATAL_ERROR "${lab_name}: 请先复制完整的 test/vendor/doctest 目录")
        endif()

        set(test_target "${lab_name}_tests")
        add_executable(${test_target} ${test_sources} ${implementation_sources})
        target_compile_features(${test_target} PRIVATE cxx_std_17)
        target_include_directories(${test_target} PRIVATE
            "${lab_directory}/src"
            "${lab_directory}/test"
            "${doctest_directory}"
        )
        set_target_properties(${test_target} PROPERTIES
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
        )
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
            target_compile_options(${test_target} PRIVATE -Wall -Wextra -Wpedantic)
        endif()

        include("${doctest_directory}/doctest.cmake")
        doctest_discover_tests(${test_target} TEST_PREFIX "${lab_name}.")
    endforeach()
endif()
