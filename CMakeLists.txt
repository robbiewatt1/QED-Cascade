cmake_minimum_required(VERSION 3.5)
project(QEDCASC)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -Wall)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

INCLUDE_DIRECTORIES(Source)
add_subdirectory(Source/IO)
add_subdirectory(Source/Physics)
add_subdirectory(Source/Particles)
add_subdirectory(Source/Tools)
add_subdirectory(Source/Exec)
add_subdirectory(Tests)
