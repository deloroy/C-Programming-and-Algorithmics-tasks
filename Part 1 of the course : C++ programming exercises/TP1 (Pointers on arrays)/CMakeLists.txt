cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{Imagine_DIR}" d)
if(NOT EXISTS "${d}")
    message(FATAL_ERROR "Error: environment variable Imagine_DIR=" "${d}")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${d}/CMake")
find_package(Imagine)

project(Pointeurs)
add_executable(Pointeurs matrice.h matrice.cpp main.cpp)
ImagineUseModules(Pointeurs Graphics)

