cmake_minimum_required(VERSION 3.21)
project(AstroViewerPlugin CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include main program plugin interface headers
include_directories(../../fits_label_vulkan/src/core)

# Include stb_image header
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Plugin source files
set(ASTRO_VIEWER_PLUGIN_SOURCE_FILES
    AstroViewerPlugin.cpp
    AstroViewerPlugin.h
)

# Create shared library
add_library(AstroViewerPlugin SHARED ${ASTRO_VIEWER_PLUGIN_SOURCE_FILES})

# Set shared library properties
set_target_properties(AstroViewerPlugin PROPERTIES
    OUTPUT_NAME "AstroViewerPlugin"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# Main program plugins directory path
set(MAIN_PROGRAM_PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../fits_label_vulkan/build/Release/plugins")

# Create plugins directory if not exists
add_custom_command(
    TARGET AstroViewerPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MAIN_PROGRAM_PLUGINS_DIR}"
    COMMENT "Creating plugins directory at ${MAIN_PROGRAM_PLUGINS_DIR}"
)

# Copy plugin DLL to main program plugins directory
add_custom_command(
    TARGET AstroViewerPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:AstroViewerPlugin> "${MAIN_PROGRAM_PLUGINS_DIR}/$<TARGET_FILE_NAME:AstroViewerPlugin>"
    COMMENT "Copying AstroViewerPlugin to ${MAIN_PROGRAM_PLUGINS_DIR}"
)

