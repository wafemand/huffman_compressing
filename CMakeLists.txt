cmake_minimum_required(VERSION 3.10)
project(huffman_utility)

set(CMAKE_CXX_STANDARD 17)

include_directories(${huffman_utility_SOURCE_DIR})
include_directories(lib/headers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -O3 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -D_DEBUG -g -O0 -fno-omit-frame-pointer -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3")

add_executable(huffman_utility main.cpp)

target_link_libraries(huffman_utility huffman)

add_subdirectory(lib)
