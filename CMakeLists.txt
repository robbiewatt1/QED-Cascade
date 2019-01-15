cmake_minimum_required(VERSION 3.5)
project(QEDCASC)

#find_package(HDF5 REQUIRED COMPONENTS C CXX)
#find_package(Boost REQUIRED COMPONENTS filesystem)
#find_package (Eigen3 3.3 REQUIRED NO_MODULE)
#include_directories(${PROJECT_SOURCE_DIR}/include ${HDF5_INCLUDE_DIRS})
#file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cpp)
#file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

set(CMAKE_CXX_STANDARD 17)

#set(INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/../lib)
#set(TOOLS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Tools)
#set(Tools_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Tools)
#set(Tools_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Tools)

INCLUDE_DIRECTORIES(Source)
add_subdirectory(Source/IO)
add_subdirectory(Source/Physics)
add_subdirectory(Source/Particles)
add_subdirectory(Source/Tools)
add_subdirectory(Source/Exec)

