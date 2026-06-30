cmake_minimum_required(VERSION 3.15)

project(ShellInCPP LANGUAGES CXX)

#C++ Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#program
add_executable(shell main.cpp)
