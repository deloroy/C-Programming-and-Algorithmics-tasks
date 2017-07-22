cmake_minimum_required(VERSION 2.6)

project (villes_les_plus_proches)



# add_executable (villes main.cpp town.cpp


# 	        point2d.cpp square.cpp quadrant.cpp)


# rm main.cpp square.cpp quadrant.cpp 
add_executable (test_villes example.cpp town.cpp town.h point2d.cpp point2d.h square.cpp square.h quadrant.cpp quadrant.h neighbors.h quadtree.h)



add_definitions(-DsrcPath="${CMAKE_CURRENT_SOURCE_DIR}")
