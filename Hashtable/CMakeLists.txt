cmake_minimum_required(VERSION 3.15)
project(Hashtable)

set(CMAKE_CXX_STANDARD 11)

add_executable(Hashtable main.cpp hash.h hash.cpp hashmap.h util.h util.cpp)


set(PROJECT_HEADERS
        hash.h
        hashmap.h
        util.h
        )

set(PROJECT_SOURCES
        main.cpp
        hash.cpp
        util.cpp
        )

set(SOURCE_FILES main.cpp hash.cpp util.cpp)