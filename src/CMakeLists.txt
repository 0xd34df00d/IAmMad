project(iammad)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

install(DIRECTORY include/ DESTINATION include/iammad FILES_MATCHING PATTERN "*.h")
