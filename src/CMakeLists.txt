project(iammad)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TESTS "Enable tests" OFF)

if(ENABLE_TESTS)
	enable_testing()

	set(CMAKE_CXX_FLAGS "-Wall -Wextra")

	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

	add_executable(basic_diff WIN32 tests/diff.cpp)
	add_test(basic_diff basic_diff)
endif()

install(DIRECTORY include/ DESTINATION include/iammad FILES_MATCHING PATTERN "*.h")
