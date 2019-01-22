cmake_minimum_required(VERSION 3.5)
project(Physics C CXX)

set(physics_source_files
	Fields/LaserField.cpp
	Fields/StaticField.cpp
	Fields/PlaneField.cpp
	Processes/NonLinearCompton.cpp
	ParticlePushers/ParticlePusher.cpp)
add_library(Physics SHARED ${physics_source_files})
target_link_libraries(Physics Tools Particles)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Fields 
					  ${CMAKE_CURRENT_SOURCE_DIR}/Processes
					  ${CMAKE_CURRENT_SOURCE_DIR}/ParticlePushers)
install(TARGETS Physics
    	RUNTIME DESTINATION bin
    	LIBRARY DESTINATION lib)
