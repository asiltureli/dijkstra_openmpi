cmake_minimum_required(VERSION 3.10)

project(DijkstraLibrary)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(DijkstraLibrary dijkstra.cpp)

target_include_directories(DijkstraLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
