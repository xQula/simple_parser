cmake_minimum_required(VERSION 3.25)
project(simple_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(simple_parser simple_parser.cpp in.txt)