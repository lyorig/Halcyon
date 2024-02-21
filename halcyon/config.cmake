
set(HALCYON_SOURCES
cleanup.cpp
debug.cpp
image_loader.cpp
input_handler.cpp
renderer.cpp
system.cpp
texture.cpp
ttf_engine.cpp
window.cpp
components/font.cpp
components/surface.cpp
other/display.cpp
other/driver.cpp
other/printing.cpp
)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
link_libraries(SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)