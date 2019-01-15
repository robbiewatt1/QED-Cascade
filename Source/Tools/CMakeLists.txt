cmake_minimum_required(VERSION 3.5)
project(Tools C CXX)

set(tools_source_files
    ThreeVector.hh
    ThreeMatrix.hh
    Numerics.hh
    Numerics.cpp
    MCTools.hh
    MCTools.cpp
    UnitsSystem.hh
    UnitsSystem.cpp)

add_library(Tools SHARED  ${tools_source_files})
target_include_directories(Tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#TARGET_COMPILE_DEFINITIONS(Tools PRIVATE SHARED_LIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}")

install(TARGETS Tools
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
