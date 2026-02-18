cmake_minimum_required(VERSION 3.22.1)
project(A2_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(A2_3 main.cpp
        GridWorld.cpp
        GridWorld.h
        ValueIteration.cpp
        ValueIteration.h
        via.cpp
        PolicyIteration.cpp
        PolicyIteration.h
        pia.cpp
        via.cpp
        via.h
        pia.cpp
        pia.h)
