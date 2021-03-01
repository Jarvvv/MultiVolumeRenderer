
add_executable(simple_volume_renderer simple_renderer_main.cpp)

target_include_directories(simple_volume_renderer PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(simple_volume_renderer PRIVATE sv::sv_core)

set(ADD_BLOCK_VOLUME_RENDERER TRUE)

if(ADD_BLOCK_VOLUME_RENDERER)
    add_executable(block_volume_renderer block_renderer_main.cpp)
    target_include_directories(block_volume_renderer PUBLIC ${CMAKE_SOURCE_DIR}/src ${MPI_INCLUDE_DIR})
    target_link_libraries(block_volume_renderer PUBLIC sv::sv_core ${MPI_LIBS})
endif()

