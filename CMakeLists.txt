cmake_minimum_required(VERSION 3.26)
project(DpllSolver)

set(CMAKE_CXX_STANDARD 11)

add_executable(DpllSolver main.cpp
        SodukuSolver.cpp
        SodukuSolver.h
        DpllSolver.cpp
        DpllSolver.h
        Helper.h
)
