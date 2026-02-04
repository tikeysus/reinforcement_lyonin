cmake_minimum_required(VERSION 4.1)
project(A1_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(A1_2 main.cpp
        Env.cpp
        Env.h
        LA_Agents.cpp
        LA_Agents.h
        UCB_Agent.cpp
        UCB_Agent.h
        Simulator.cpp
        Simulator.h)
