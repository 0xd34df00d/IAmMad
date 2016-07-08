project(iammad)
cmake_minimum_required(VERSION 3.0)

install(DIRECTORY include/ DESTINATION include/iammad FILES_MATCHING PATTERN "*.h")
