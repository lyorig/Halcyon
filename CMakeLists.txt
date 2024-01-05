cmake_minimum_required(VERSION 3.22.1)

if(MSVC)
  add_compile_options(/Wall)
else()
  add_compile_options(-Wall -Wextra)
endif()

if (APPLE)
  include_directories(/opt/homebrew/include)
  link_directories(/opt/homebrew/lib)
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES Debug AND NOT MSVC)
  add_compile_options(-flto)
  add_link_options(-flto)
endif()

add_compile_options(-fsanitize=undefined)
add_link_options(-fsanitize=undefined)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_libraries(SDL2 SDL2_image SDL2_ttf SDL2_mixer)

project(Halcyon VERSION 0.1 LANGUAGES CXX)

find_package(SDL2 REQUIRED)

add_executable(Halcyon)
add_executable(Haltest)

set_property(TARGET Halcyon PROPERTY CXX_STANDARD 23)
set_property(TARGET Haltest PROPERTY CXX_STANDARD 23)

list(APPEND Halcyon_sources
halcyon/src/debug.cpp
halcyon/src/engine.cpp
halcyon/src/image_loader.cpp
halcyon/src/input_handler.cpp
halcyon/src/mixer.cpp
halcyon/src/renderer.cpp
halcyon/src/texture.cpp
halcyon/src/ttf_engine.cpp
halcyon/src/window.cpp
halcyon/src/components/chunk.cpp
halcyon/src/components/font.cpp
halcyon/src/components/music.cpp
halcyon/src/components/surface.cpp
halcyon/src/internal/subsystem.cpp
halcyon/src/internal/printing.cpp)

target_sources(Halcyon PRIVATE
main.cpp
src/game.cpp
${Halcyon_sources})

target_sources(Haltest PRIVATE
test/test.cpp
${Halcyon_sources})