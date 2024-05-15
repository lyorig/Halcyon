# Intended for testing. Do not use, include config.cmake instead.

cmake_minimum_required(VERSION 3.22.0)

project(HalTest VERSION 1.0 LANGUAGES CXX)

# Configure Halcyon.
include(config.cmake)
include(CTest)

include_directories(${HALCYON_INCLUDE_DIRS})
link_libraries(${HALCYON_LIBRARIES})

# These tests require assertions present in Halcyon. Make sure they're on.
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(HAL_DEBUG_ENABLED)
endif()

add_library(Halcyon OBJECT ${HALCYON_SOURCES})

# Tests.

add_executable(HalTest test/main.cpp $<TARGET_OBJECTS:Halcyon>)

set(ExeName $<TARGET_FILE:HalTest>)

add_test(NAME AssertFail        COMMAND ${ExeName} --assert-fail)
add_test(NAME WindowResize      COMMAND ${ExeName} --window-resize)
add_test(NAME BasicInit         COMMAND ${ExeName} --basic-init)
add_test(NAME Clipboard         COMMAND ${ExeName} --clipboard)
add_test(NAME SurfaceColor      COMMAND ${ExeName} --surface-color)
add_test(NAME Events            COMMAND ${ExeName} --events)
add_test(NAME TTFInit           COMMAND ${ExeName} --ttf-init)
add_test(NAME RValues           COMMAND ${ExeName} --rvalues)
add_test(NAME Scaler            COMMAND ${ExeName} --scaler)
add_test(NAME Outputter         COMMAND ${ExeName} --outputter)
add_test(NAME PngCheck          COMMAND ${ExeName} --png-check)
add_test(NAME Metaprogramming   COMMAND ${ExeName} --metaprogramming)
add_test(NAME InvalidBuffer     COMMAND ${ExeName} --invalid-buffer)
add_test(NAME InvalidTexture    COMMAND ${ExeName} --invalid-texture)
add_test(NAME InvalidEvent      COMMAND ${ExeName} --invalid-event)

# Set which tests are expected to fail.
set_tests_properties(AssertFail InvalidBuffer InvalidTexture InvalidEvent PROPERTIES WILL_FAIL TRUE)

# Examples.

set(EXAMPLE_SOURCES
invertor.cpp
message_box.cpp
metaprogramming.cpp
safety.cpp
text_render.cpp)

list(TRANSFORM EXAMPLE_SOURCES PREPEND examples/)

foreach(FILE IN LISTS EXAMPLE_SOURCES)
    get_filename_component(TGT_NAME ${FILE} NAME_WE)
    add_executable(${TGT_NAME} ${FILE} $<TARGET_OBJECTS:Halcyon>)
    set_target_properties(${TGT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
endforeach()