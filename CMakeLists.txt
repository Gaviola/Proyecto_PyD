cmake_minimum_required(VERSION 3.27)
project(Proyecto_PyD)

set(CMAKE_CXX_STANDARD 17)
find_package(MPI REQUIRED)

add_executable(MPI paralelo/reader.cpp
        paralelo/mergeParalelo.cpp
        paralelo/utils.cpp)

target_link_libraries(MPI PRIVATE MPI::MPI_CXX)