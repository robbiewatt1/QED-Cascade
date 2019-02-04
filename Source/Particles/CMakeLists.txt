cmake_minimum_required(VERSION 3.5)
project(Particles C CXX)

set(particles_source_files
    Particle.cpp
    ParticleList.cpp)
set(particles_header_files
    Particle.hh
    ParticleList.hh)

add_library(Particles SHARED  ${particles_source_files})
target_link_libraries(Particles Tools)
target_include_directories(Particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Particles
    	LIBRARY DESTINATION lib)

install(FILES ${particles_header_files} 
	DESTINATION include)
