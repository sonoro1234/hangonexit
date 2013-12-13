cmake_minimum_required(VERSION 2.8)
set(PROYECTO lockedqueue)
PROJECT(${PROYECTO} CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -std=c++11")
add_executable(${PROYECTO} ${PROYECTO}.cpp)
add_library(${PROYECTO}dll SHARED ${PROYECTO}dll.cpp)
 