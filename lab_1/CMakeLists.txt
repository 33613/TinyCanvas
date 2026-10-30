cmake_minimum_required(VERSION 3.16)
project(TinyCanvasLab1 LANGUAGES CXX)

include(CTest)
if(BUILD_TESTING)
    # 数据层均为头文件；验收交互与测试入口集中在 test 下。
    add_executable(graph_tests
        test/graph_test.cpp
        test/support/lab1_console.cpp
    )
    target_compile_features(graph_tests PRIVATE cxx_std_17)
    set_target_properties(graph_tests PROPERTIES CXX_EXTENSIONS OFF)
    target_include_directories(graph_tests PRIVATE src test/vendor/doctest)

    # 测试框架及其 CMake 适配脚本统一作为测试依赖保存。
    include("${CMAKE_CURRENT_SOURCE_DIR}/test/vendor/doctest/doctest.cmake")
    doctest_discover_tests(graph_tests)
endif()
