# Halcyon CMake configuration. Include this and you should be good to go.

# Configure lyoSTL.
include(${CMAKE_CURRENT_LIST_DIR}/lib/lyoSTL/config.cmake)

# Make sure SDL is present on the system.
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

# Include directores.
set(HALCYON_INCLUDE_DIRS ${LYO_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR}/include/)

# Sources.
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

# Libraries.
set(HALCYON_LIBRARIES
SDL2::SDL2main
SDL2::SDL2
SDL2_image::SDL2_image
SDL2_ttf::SDL2_ttf
)

# Halcyon uses C++23 features.
set(CMAKE_CXX_STANDARD 23)