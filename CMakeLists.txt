cmake_minimum_required(VERSION 3.22.1)

# Create the project.
project(HalShowcase VERSION 0.1 LANGUAGES CXX)

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
endif()

include(halcyon/config.cmake)

add_executable(HalMain 
${CMAKE_CURRENT_SOURCE_DIR}/test/hal_main.cpp  ${HALCYON_SOURCES}
)

set_target_properties(HalMain PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/bin/
CXX_STANDARD 23)

target_link_libraries(HalMain ${HALCYON_LIBRARIES})