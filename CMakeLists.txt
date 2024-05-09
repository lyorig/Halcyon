# Intended for testing. Do not use, include config.cmake instead.

cmake_minimum_required(VERSION 3.22.0)

project(HalTest VERSION 1.0 LANGUAGES CXX)

# Configure Halcyon.
include(config.cmake)
include(CTest)

include_directories(${HALCYON_INCLUDE_DIRS})
link_libraries(${HALCYON_LIBRARIES})

# Tests.

add_executable(HalTest test/main.cpp ${HALCYON_SOURCES})

# These tests require assertions present in Halcyon. Make sure they're on.
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(HalTest PRIVATE HAL_DEBUG_ENABLED)
endif()

set(ExeName $<TARGET_FILE:HalTest>)

add_test(NAME AssertFail        COMMAND ${ExeName} --assert-fail)
add_test(NAME WindowResize      COMMAND ${ExeName} --window-resize)
add_test(NAME BasicInit         COMMAND ${ExeName} --basic-init)
add_test(NAME InvalidBuffer     COMMAND ${ExeName} --invalid-buffer)
add_test(NAME Clipboard         COMMAND ${ExeName} --clipboard)
add_test(NAME SurfaceColor      COMMAND ${ExeName} --surface-color)
add_test(NAME InvalidTexture    COMMAND ${ExeName} --invalid-texture)
add_test(NAME QuitEvent         COMMAND ${ExeName} --quit-event)
add_test(NAME TTFInit           COMMAND ${ExeName} --ttf-init)
add_test(NAME RValues           COMMAND ${ExeName} --rvalues)
add_test(NAME Scaler            COMMAND ${ExeName} --scaler)
add_test(NAME Outputter         COMMAND ${ExeName} --outputter)
add_test(NAME PngCheck          COMMAND ${ExeName} --png-check)
add_test(NAME Metaprogramming   COMMAND ${ExeName} --metaprogramming)

# Set which tests are expected to fail.
set_tests_properties(AssertFail InvalidBuffer InvalidTexture PROPERTIES WILL_FAIL TRUE)