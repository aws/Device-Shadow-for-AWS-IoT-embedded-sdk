cmake_minimum_required(VERSION 3.15)

project(Device-Shadow LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/shadowFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library(device_shadow_for_aws INTERFACE)

target_sources(device_shadow_for_aws INTERFACE ${SHADOW_SOURCES})

target_include_directories(device_shadow_for_aws INTERFACE ${SHADOW_INCLUDE_PUBLIC_DIRS})

message(STATUS "device_shadow_for_aws library requires SHADOW_DO_NOT_USE_CUSTOM_CONFIG definition or a shadow_config.h file.")
