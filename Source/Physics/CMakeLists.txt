cmake_minimum_required(VERSION 3.5)
project(PhysicsQED C CXX)

set(physics_source_files
	Fields/EMField/GaussianEMField.cpp
	Fields/EMField/StaticEMField.cpp
	Fields/EMField/PlaneEMField.cpp
	Processes/NonLinearCompton.cpp
	Processes/NonLinearBreitWheeler.cpp
	ParticlePushers/ParticlePusher.cpp
    ParticlePushers/ParticlePusher.cpp
    ParticlePushers/LandauPusher.cpp
    ParticlePushers/LorentzPusher.cpp)
set(physics_header_files
	Fields/EMField/EMField.hh
	Fields/EMField/GaussianEMField.hh
    Fields/EMField/StaticEMField.hh
    Fields/EMField/PlaneEMField.hh
    Processes/Process.hh
	Processes/NonLinearCompton.hh
	Processes/NonLinearBreitWheeler.hh
    ParticlePushers/ParticlePusher.hh
    ParticlePushers/LandauPusher.hh
    ParticlePushers/LorentzPusher.hh)
add_library(PhysicsQED SHARED ${physics_source_files})
target_link_libraries(PhysicsQED Tools Particles)
target_include_directories(PhysicsQED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Fields/EMField 
					  ${CMAKE_CURRENT_SOURCE_DIR}/Processes
					  ${CMAKE_CURRENT_SOURCE_DIR}/ParticlePushers)
install(TARGETS PhysicsQED
    	LIBRARY DESTINATION lib)
install(FILES ${physics_header_files}
        DESTINATION include)
