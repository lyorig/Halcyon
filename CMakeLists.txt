cmake_minimum_required(VERSION 3.22.1)

# Create the project.
project(HalodaQuest VERSION 0.1 LANGUAGES CXX)

include(lyo/config.cmake)
include(halcyon/config.cmake)
include(AL++/config.cmake)

# Set appropriate warnings and directories.
if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Enable link-time optimisation in release.
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
	if (MSVC)
		add_compile_options(/GL)
		add_link_options(/LCTG)
	else()
		add_compile_options(-flto)
		add_link_options(-flto)
	endif()

# We're in Debug mode. Enable extra checks.
elseif(NOT MSVC)
	add_compile_options(-fsanitize=address -fsanitize=undefined)
	add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(HalChess WIN32)
add_executable(HalTest WIN32)

set_property(TARGET HalChess PROPERTY CXX_STANDARD 23)
set_property(TARGET HalTest PROPERTY CXX_STANDARD 23)

target_sources(HalChess PRIVATE
chess/main.cpp
${HALCYON_SOURCES}
)

target_sources(HalTest PRIVATE
test/main.cpp
${HALCYON_SOURCES}
${ALPP_SOURCES}
)

target_compile_definitions(HalChess PRIVATE HAL_INTEGRAL_COORD)