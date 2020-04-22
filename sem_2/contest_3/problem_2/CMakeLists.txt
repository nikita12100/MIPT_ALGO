cmake_minimum_required(VERSION 3.9)
project(TravelingsSalesman)

set(CMAKE_CXX_STANDARD 14)

add_executable(TravelingsSalesman main.cpp MST.cpp Graph.cpp TSP_LenPath.cpp TSP_MST_MinPat.cpp Dots.cpp Checker.cpp)