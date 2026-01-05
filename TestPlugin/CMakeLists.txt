cmake_minimum_required(VERSION 3.21)
project(TestPlugin CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含主程序的插件接口头文件
include_directories(../../fits_label_vulkan/src/core)

# 示例插件源文件
set(TEST_PLUGIN_SOURCE_FILES
    TestPlugin.cpp
    TestPlugin.h
)

# 创建动态库
add_library(TestPlugin SHARED ${TEST_PLUGIN_SOURCE_FILES})

# 设置动态库的属性
set_target_properties(TestPlugin PROPERTIES
    OUTPUT_NAME "TestPlugin"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
