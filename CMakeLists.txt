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
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(HalTest PRIVATE HAL_DEBUG_ENABLED HAL_DEBUG_ADVANCED)
endif()

add_test(NAME AssertFail        COMMAND $<TARGET_FILE:HalTest> --assert-fail)
add_test(NAME WindowResize      COMMAND $<TARGET_FILE:HalTest> --window-resize)
add_test(NAME BasicInit         COMMAND $<TARGET_FILE:HalTest> --basic-init)
add_test(NAME InvalidBuffer     COMMAND $<TARGET_FILE:HalTest> --invalid-buffer)
add_test(NAME Clipboard         COMMAND $<TARGET_FILE:HalTest> --clipboard)
add_test(NAME SurfaceColor      COMMAND $<TARGET_FILE:HalTest> --surface-color)
add_test(NAME InvalidTexture    COMMAND $<TARGET_FILE:HalTest> --invalid-texture)
add_test(NAME QuitEvent         COMMAND $<TARGET_FILE:HalTest> --quit-event)
add_test(NAME TTFInit           COMMAND $<TARGET_FILE:HalTest> --ttf-init)
add_test(NAME RValues           COMMAND $<TARGET_FILE:HalTest> --rvalues)


# Set which tests are expected to fail.
set_tests_properties(AssertFail InvalidBuffer InvalidTexture PROPERTIES WILL_FAIL TRUE)