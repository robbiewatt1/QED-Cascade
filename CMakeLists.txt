cmake_minimum_required(VERSION 3.5)
project(QEDCASC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -fopenmp")
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/Install")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

option(BUILD_TESTS "Build test executables" OFF)
option(BUILD_GEANT "Build geant executables" OFF)
option(BUILD_TENSORFLOW "Build the geant4 tensorflow package" OFF)

INCLUDE_DIRECTORIES(Source)
add_subdirectory(Source/IO)
add_subdirectory(Source/Physics)
add_subdirectory(Source/Particles)
add_subdirectory(Source/Tools)
add_subdirectory(Source/Exec)

if(BUILD_TESTS)
	add_subdirectory(Tests)
endif(BUILD_TESTS)

if(BUILD_GEANT)
	add_subdirectory(Source/Geant)
endif(BUILD_GEANT)
