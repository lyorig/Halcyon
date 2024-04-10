# Configure lyoSTL.
include(${CMAKE_CURRENT_LIST_DIR}/lib/lyoSTL/config.cmake)

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

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

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

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)