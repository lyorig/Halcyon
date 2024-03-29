cmake_minimum_required(VERSION 3.22.1)

# Create the project.
project(HalodaQuest VERSION 0.1 LANGUAGES CXX)

include (quest/config.cmake)

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

elseif(NOT MSVC)
	add_compile_options(-fsanitize=undefined)
	add_link_options(-fsanitize=undefined)
endif()

add_executable(HalQuest WIN32)

set_target_properties(HalQuest PROPERTIES
CXX_STANDARD 23
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

target_sources(HalQuest PRIVATE
${HALCYON_SOURCES}
${QUEST_SOURCES}
${ALPP_SOURCES}
)