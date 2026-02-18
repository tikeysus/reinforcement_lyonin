cmake_minimum_required(VERSION 3.22.1)
project(A1_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(A1_1 main.cpp
        Env.h
        Env.cpp
        Agent.cpp
        Agent.h
        Simulator.cpp
        Simulator.h)
