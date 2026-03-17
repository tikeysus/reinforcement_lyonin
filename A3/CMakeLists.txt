cmake_minimum_required(VERSION 3.22.1)
project(A3_3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_SOURCES
        Gridworld.cpp
        RLCommon.cpp)

add_executable(mc
        mc.cpp
        MonteCarloControl.cpp
        MonteCarloControl.h
        ${COMMON_SOURCES}
        Gridworld.h
        RLCommon.h)

add_executable(sarsa
        sarsa.cpp
        SarsaLearner.cpp
        SarsaLearner.h
        ${COMMON_SOURCES}
        Gridworld.h
        RLCommon.h)

add_executable(q
        q.cpp
        QLearning.cpp
        QLearning.h
        ${COMMON_SOURCES}
        Gridworld.h
        RLCommon.h)

add_executable(expected
        expected.cpp
        ExpectedSarsa.cpp
        ExpectedSarsa.h
        ${COMMON_SOURCES}
        Gridworld.h
        RLCommon.h)

add_executable(doubleq
        doubleq.cpp
        DoubleQLearning.cpp
        DoubleQLearning.h
        ${COMMON_SOURCES}
        Gridworld.h
        RLCommon.h)
