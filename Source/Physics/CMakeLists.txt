cmake_minimum_required(VERSION 3.5)
project(Physics C CXX)

set(physics_source_files
	Fields/EMField/GaussianEMField.cpp
	Fields/EMField/StaticEMField.cpp
	Fields/EMField/PlaneEMField.cpp
	Fields/RadiationField/ThermalRadField.cpp
	Processes/NonLinearCompton.cpp
	Processes/Process.cpp
	Processes/ThermalBW.cpp
	ParticlePushers/ParticlePusher.cpp)
add_library(Physics SHARED ${physics_source_files})
target_link_libraries(Physics Tools Particles)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Fields/EMField 
					  ${CMAKE_CURRENT_SOURCE_DIR}/Fields/RadiationField
					  ${CMAKE_CURRENT_SOURCE_DIR}/Processes
					  ${CMAKE_CURRENT_SOURCE_DIR}/ParticlePushers)
install(TARGETS Physics
    	RUNTIME DESTINATION bin
    	LIBRARY DESTINATION lib)
