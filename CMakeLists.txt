# Intended for testing. Do not use, include config.cmake instead.

cmake_minimum_required(VERSION 3.22.0)

project(HalTest VERSION 1.0 LANGUAGES CXX)

# Configure Halcyon.
include(config.cmake)
include(CTest)

# Setup compiler for tests.
include_directories(${HALCYON_INCLUDE_DIRS})
link_libraries(${HALCYON_LIBRARIES})

# Create the test runner.
add_executable(HalTest test/main.cpp ${HALCYON_SOURCES})

# These tests use assertions present in Halcyon. Make sure they're on.
target_compile_definitions(HalTest PRIVATE HAL_DEBUG_ENABLED)

add_test(NAME AssertFail COMMAND $<TARGET_FILE:HalTest> --assert-fail)
add_test(NAME WindowResize COMMAND $<TARGET_FILE:HalTest> --window-resize)
add_test(NAME BasicInit COMMAND $<TARGET_FILE:HalTest> --basic-init)
add_test(NAME InvalidBuffer COMMAND $<TARGET_FILE:HalTest> --invalid-buffer)
add_test(NAME Clipboard COMMAND $<TARGET_FILE:HalTest> --clipboard)
add_test(NAME SurfaceColor COMMAND $<TARGET_FILE:HalTest> --surface-color)

# Set which tests are expected to fail.
set_tests_properties(AssertFail InvalidBuffer PROPERTIES WILL_FAIL TRUE)