cmake_minimum_required(VERSION 3.5)
project(Physics C CXX)

set(physics_source_files
	Fields/EMField/GaussianEMField.cpp
	Fields/EMField/StaticEMField.cpp
	Fields/EMField/PlaneEMField.cpp
	Processes/NonLinearCompton.cpp
	ParticlePushers/ParticlePusher.cpp)
set(physics_header_files
	Fields/EMField/EMField.hh
	Fields/EMField/GaussianEMField.hh
        Fields/EMField/StaticEMField.hh
        Fields/EMField/PlaneEMField.hh
	Processes/NonLinearCompton.hh
        ParticlePushers/ParticlePusher.hh)

add_library(Physics SHARED ${physics_source_files})
target_link_libraries(Physics Tools Particles)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Fields/EMField 
					  ${CMAKE_CURRENT_SOURCE_DIR}/Processes
					  ${CMAKE_CURRENT_SOURCE_DIR}/ParticlePushers)
install(TARGETS Physics
    	LIBRARY DESTINATION lib)
install(FILES ${physics_header_files}
        DESTINATION include)
