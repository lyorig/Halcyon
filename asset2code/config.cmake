include(halcyon/config.cmake)

add_executable(asset2code ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp ${HALCYON_SOURCES})

target_link_libraries(asset2code ${HALCYON_LIBRARIES})

set_target_properties(asset2code PROPERTIES
CXX_STANDARD 23
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/
)