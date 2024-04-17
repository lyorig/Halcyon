# Halcyon CMake configuration. Include this and you should be good to go.

# Make sure SDL is present on the system.
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

# Include directores.
set(HALCYON_INCLUDE_DIRS ${LYOSTL_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR}/include/)

# Sources.
set(HALCYON_SOURCES
event/handler.cpp
event/types.cpp
internal/accessor.cpp
types/templates.cpp
utility/printing.cpp
utility/strutil.cpp
utility/timer.cpp
video/display.cpp
video/renderer.cpp
video/texture.cpp
video/window.cpp
keyboard.cpp
mouse.cpp
context.cpp
driver.cpp
debug.cpp
image.cpp
surface.cpp
ttf.cpp
video.cpp
)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(APPEND HALCYON_SOURCES ${LYOSTL_SOURCES})

# Libraries.
set(HALCYON_LIBRARIES
SDL2::SDL2main
SDL2::SDL2
SDL2_image::SDL2_image
SDL2_ttf::SDL2_ttf
)

# Halcyon uses C++23 features.
set(CMAKE_CXX_STANDARD 23)