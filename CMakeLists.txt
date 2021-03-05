cmake_minimum_required(VERSION 3.17)
project(VolumeRenderer)

set(CMAKE_CXX_STANDARD 14)
#enable_language(Fortran)

set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/third_party;${CMAKE_MODULE_PATH}")

#MPI
set(MPI_ROOT_DIR ${CMAKE_SOURCE_DIR}/third_party/MPI)
set(MPI_INCLUDE_DIR ${MPI_ROOT_DIR}/Include)
set(MPI_LIB_DIR ${MPI_ROOT_DIR}/Lib/x86)
set(MPI_LIBS ${MPI_LIB_DIR}/msmpi.lib;)


set(VolumeCompression_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/VolumeCompression/include)

include_directories(./include ./third_party/MPI/Include ./third_party/glad/include ./third_party/glfw/include ./third_party/glm ./third_party/VolumeCompression/include)

add_subdirectory(test)

add_subdirectory(src)

add_subdirectory(renderer)

add_subdirectory(third_party/glfw)

#add_subdirectory(third_party/VolumeCompression)


