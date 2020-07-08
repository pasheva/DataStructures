cmake_minimum_required(VERSION 3.15)
project(Graphs2)

set(CMAKE_CXX_STANDARD 11)

add_executable(Graphs2 main.cpp graph.h dist.h dist.cpp osm.h osm.cpp tinyxml2.h tinyxml2.cpp dijkstra.h dijkstra.cpp)

set(PROJECT_HEADERS
        graph.h
        dist.h
        osm.h
        tinyxml2.h
        dijkstra.h
        )

set(PROJECT_SOURCES
        main.cpp
        dist.cpp
        osm.cpp
        tinyxml2.cpp
        dijkstra.cpp
        )

set(SOURCE_FILES main.cpp dist.cpp osm.cpp tinyxml2.cpp dijkstra.cpp)