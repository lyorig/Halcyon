# Halcyon CMake configuration. Include this and you should be good to go.

# Make sure SDL is present on the system.
find_package(SDL3 REQUIRED CONFIG)

# Include directores.
set(HALCYON_INCLUDE_DIRS ${LYOSTL_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR}/include/)

# Sources.
set(HALCYON_SOURCES
event/handler.cpp
event/keyboard.cpp
event/mouse.cpp
event/types.cpp
internal/accessor.cpp
types/templates.cpp
utility/printing.cpp
utility/strutil.cpp
utility/timer.cpp
video/display.cpp
video/driver.cpp
video/renderer.cpp
video/texture.cpp
video/window.cpp
context.cpp
debug.cpp
surface.cpp
video.cpp
)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(APPEND HALCYON_SOURCES ${LYOSTL_SOURCES})

# Libraries.
set(HALCYON_LIBRARIES
SDL3::SDL3
)

# Halcyon uses C++23 features.
set(CMAKE_CXX_STANDARD 23)