cmake_minimum_required(VERSION 3.0)

project(obstacle_avoidance)

include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/../../../lib/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall")

add_library(
    obstacle_avoidance
    SHARED
    obstacle_avoidance/obstacle_avoidance.c
    math/geometry/circles.c
    math/geometry/discrete_circles.c
    math/geometry/lines.c
    math/geometry/polygon.c
    math/geometry/vect_base.c
    math/vect2/vect2.c
    ${CMAKE_SOURCE_DIR}/../../../lib/error/error.c
    )


target_link_libraries(
    obstacle_avoidance
    m
    )
