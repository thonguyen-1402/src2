cmake_minimum_required(VERSION 3.0)
project(codec_project)
include_directories(${CMAKE_BUILD_DIRECTORY}/../src2)

set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIRECTORY}/../src2)

add_executable(codec

${CMAKE_SOURCE_DIR}/../src2/encoder.cpp
${CMAKE_SOURCE_DIR}/../src2/octree.h
)