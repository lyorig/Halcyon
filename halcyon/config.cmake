
set(HALCYON_SOURCES
debug.cpp
engine.cpp
image_loader.cpp
input_handler.cpp
renderer.cpp
texture.cpp
ttf_engine.cpp
window.cpp
components/font.cpp
components/surface.cpp
other/printing.cpp
other/subsystem.cpp
)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
link_libraries(SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)