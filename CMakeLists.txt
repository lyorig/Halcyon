cmake_minimum_required(VERSION 3.22.1)

# Create the project.
project(HalodaQuest VERSION 0.1 LANGUAGES CXX)

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
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

# The HalodaQuest game.
include (quest/config.cmake)

add_executable(HalQuest WIN32)

set_target_properties(HalQuest PROPERTIES
CXX_STANDARD 23
RUNTIME_OUTPUT_DIRECTORY ${QUEST_BINARY_DIR}
)

target_sources(HalQuest PRIVATE
${HALCYON_SOURCES}
${QUEST_SOURCES}
)

target_link_libraries(HalQuest ${HALCYON_LIBRARIES})

# The asset to color array convertor.
include(asset2code/config.cmake)

add_executable(asset2code)

set_target_properties(asset2code PROPERTIES
CXX_STANDARD 23
RUNTIME_OUTPUT_DIRECTORY ${QUEST_BINARY_DIR}
)

target_sources(asset2code PRIVATE
${HALCYON_SOURCES}
${ASSET2CODE_SOURCES}
)

target_link_libraries(asset2code ${HALCYON_LIBRARIES})