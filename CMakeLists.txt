cmake_minimum_required(VERSION 3.10)

project(DijkstraOpenMPI VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(MPI REQUIRED)

add_subdirectory(dijkstra)

add_executable(DijkstraApp main.cpp)

target_link_libraries(DijkstraApp DijkstraLibrary)

if(MPI_FOUND)
    target_link_libraries(DijkstraApp MPI::MPI_CXX)
endif()
