cmake_minimum_required(VERSION 3.22.1)

project(Halcyon VERSION 0.1 LANGUAGES CXX)

# Set appropriate warnings and directories.
if(MSVC)
	add_compile_options(/W4)
	include_directories(C:/dev/include)
	link_directories(C:/dev/lib)
else()
	if(APPLE)
		include_directories(/opt/homebrew/include)
		link_directories(/opt/homebrew/lib)
	endif()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Enable link-time optimisation.
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
	if (MSVC)
		add_compile_options(/GL)
		add_link_options(/LCTG)
	else()
		add_compile_options(-flto)
		add_link_options(-flto)
	endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_libraries(SDL2 SDL2main SDL2_image SDL2_ttf SDL2_mixer)

add_executable(Halcyon WIN32)
add_executable(Haltest WIN32)

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