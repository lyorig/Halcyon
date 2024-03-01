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
		add_compile_options(-flto -march=native)
		add_link_options(-flto)
	endif()

# We're in Debug mode. Enable extra checks.
elseif(NOT MSVC)
	# add_compile_options(-fsanitize=address -fsanitize=undefined)
	# add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(HalQuest WIN32)

set_property(TARGET HalQuest PROPERTY CXX_STANDARD 23)

target_sources(HalQuest PRIVATE
quest/main.cpp
quest/src/entities.cpp
quest/src/game.cpp
${HALCYON_SOURCES}
${ALPP_SOURCES}
)