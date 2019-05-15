cmake_minimum_required(VERSION 3.13)
project(LSMTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(LSMTree main.cpp LsmTree.cpp LsmTree.h BloomFilter.cpp BloomFilter.h SortetStringTable.cpp SortetStringTable.h)