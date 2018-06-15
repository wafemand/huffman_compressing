cmake_minimum_required(VERSION 3.10)
project(huffman_lib)


set(CMAKE_CXX_STANDARD 17)

include_directories(${huffman_lib_SOURCE_DIR})
include_directories(testing)
include_directories(headers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -O3 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -D_DEBUG -g -O0 -fno-omit-frame-pointer -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3 -fsanitize=address,undefined")

add_library(huffman STATIC
        headers/huffman.h
        src/Tree.cpp
        headers/Tree.h
        headers/BitFileWriter.h
        headers/BitCode.h
        headers/BitFileReader.h
        headers/exceptions.h
        src/huffman.cpp
        headers/Stat.h)


add_executable(huffman_testing
        testing/gtest/gtest.h
        testing/gtest/gtest-all.cc
        testing/gtest/gtest_main.cc
        testing/huffman_testing.cpp)

target_link_libraries(huffman_testing huffman -lpthread)