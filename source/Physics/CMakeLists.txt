cmake_minimum_required(VERSION 3.5)
project(Physics C CXX)

set(physics_source_files
    LaserField.hh
    LaserField.cpp
    NonLinearCompton.hh
    NonLinearCompton.cpp
    ParticlePusher.hh
    ParticlePusher.cpp)
add_library(Physics SHARED ${physics_source_files})
target_link_libraries(Physics Tools IO Particles)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Physics
	COMPONENT application
    	RUNTIME DESTINATION bin
    	LIBRARY DESTINATION lib)
