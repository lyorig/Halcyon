cmake_minimum_required(VERSION 3.22.1)

# Create the project.
project(HalodaQuest VERSION 0.1 LANGUAGES CXX)

include(lyoSTL/config.cmake)
include(halcyon/config.cmake)
include(quest/config.cmake)

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

	# add_compile_definitions(LYO_USE_FAST_TYPES)
else()
	# We're in Debug mode. Enable extra checks.
	add_compile_options(-fsanitize=address -fsanitize=undefined)
	add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(HalodaQuest WIN32)
add_executable(HQtest WIN32)

set_property(TARGET HalodaQuest PROPERTY CXX_STANDARD 23)
set_property(TARGET HQtest PROPERTY CXX_STANDARD 23)

target_sources(HalodaQuest PRIVATE
main.cpp
${QUEST_SOURCES}
${HALCYON_SOURCES})

target_sources(HQtest PRIVATE
test/test.cpp
${HALCYON_SOURCES})

target_link_libraries(HalodaQuest ${HALCYON_LIBRARIES})
target_link_libraries(HQtest ${HALCYON_LIBRARIES})