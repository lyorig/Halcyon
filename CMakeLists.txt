cmake_minimum_required(VERSION 3.22.1)

if(MSVC)
  add_compile_options(/Wall)
else()
  add_compile_options(-Wall)
endif()

if (APPLE)
  include_directories(/opt/homebrew/include)
  link_directories(/opt/homebrew/lib)
endif()

if (CMAKE_BUILD_TYPE MATCHES Release AND NOT MSVC)
  add_compile_options(-flto)
endif()

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

project(Halcyon VERSION 0.1 LANGUAGES CXX)

find_package(SDL2 REQUIRED)

add_executable(Halcyon)

set_property(TARGET Halcyon PROPERTY CXX_STANDARD 23)

target_sources(Halcyon PRIVATE
main.cpp
game.cpp
app.cpp
halcyon/src/debug.cpp
halcyon/src/engine.cpp
halcyon/src/image_loader.cpp
halcyon/src/input_handler.cpp
halcyon/src/mixer.cpp
halcyon/src/spritesheet.cpp
halcyon/src/texture.cpp
halcyon/src/ttf_engine.cpp
halcyon/src/window.cpp
halcyon/src/components/chunk.cpp
halcyon/src/components/font.cpp
halcyon/src/components/music.cpp
halcyon/src/components/renderer.cpp
halcyon/src/components/surface.cpp
halcyon/src/internal/subsystem.cpp
halcyon/src/internal/printing.cpp)

target_include_directories(Halcyon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Halcyon SDL2 SDL2_image SDL2_ttf SDL2_mixer)