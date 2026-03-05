cmake_minimum_required(VERSION 3.15)

project(DeviceShadow LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/shadowFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library( aws_device_shadow )


target_sources( aws_device_shadow PUBLIC ${SHADOW_SOURCES} )

target_include_directories( aws_device_shadow PUBLIC ${SHADOW_INCLUDE_PUBLIC_DIRS} )

if(STANDALONE_TEST_BUILD_UNIX)
    target_compile_definitions( aws_device_shadow PUBLIC SHADOW_DO_NOT_USE_CUSTOM_CONFIG=1 )
endif()
