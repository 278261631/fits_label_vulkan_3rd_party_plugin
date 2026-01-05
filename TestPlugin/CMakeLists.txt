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

# 主程序的plugins目录路径
set(MAIN_PROGRAM_PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../fits_label_vulkan/build/Release/plugins")

# 创建plugins目录（如果不存在）
add_custom_command(
    TARGET TestPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MAIN_PROGRAM_PLUGINS_DIR}"
    COMMENT "Creating plugins directory at ${MAIN_PROGRAM_PLUGINS_DIR}"
)

# 拷贝插件DLL到主程序的plugins目录
add_custom_command(
    TARGET TestPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TestPlugin> "${MAIN_PROGRAM_PLUGINS_DIR}/$<TARGET_FILE_NAME:TestPlugin>"
    COMMENT "Copying TestPlugin to ${MAIN_PROGRAM_PLUGINS_DIR}"
)


