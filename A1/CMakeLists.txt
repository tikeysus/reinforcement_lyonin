cmake_minimum_required(VERSION 4.1)
project(A1)

set(CMAKE_CXX_STANDARD 20)

add_executable(A1 main.cpp
        Env.h
        Env.cpp
        Agent.cpp
        Agent.h
        Simulator.cpp
        Simulator.h)
