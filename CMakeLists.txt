cmake_minimum_required(VERSION 3.22.0)

project(Halcyon VERSION 1.0 LANGUAGES CXX)

# Configure lyoSTL.
include(${CMAKE_CURRENT_SOURCE_DIR}/lib/lyoSTL/config.cmake)
include(CTest)

set(HALCYON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)

set(HALCYON_SOURCES
internal/accessor.cpp
other/clipboard.cpp
other/display.cpp
other/driver.cpp
other/keyboard_mouse.cpp
other/printing.cpp
types/events.cpp
cleanup.cpp
debug.cpp
events.cpp
images.cpp
renderer.cpp
texture.cpp
ttf.cpp
window.cpp
surface.cpp
)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

set(HALCYON_LIBRARIES
SDL2::SDL2main
SDL2::SDL2
SDL2_image::SDL2_image
SDL2_ttf::SDL2_ttf
)

set(CMAKE_CXX_STANDARD 23)

# Run tests.
include_directories(${HALCYON_INCLUDE_DIR})
link_libraries(${HALCYON_LIBRARIES})

add_executable(HalTest_Event test/event.cpp ${HALCYON_SOURCES})

enable_testing()

add_test(NAME Event COMMAND $<TARGET_FILE:HalTest_Event>)