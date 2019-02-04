cmake_minimum_required(VERSION 3.5)
project(GeantQED C CXX)
find_package(Geant4 REQUIRED)

set(geant_source_files
    G4NonLinearCompton.cpp)
set(geant_header_files
    G4NonLinearCompton.hh)

add_library(GeantQED SHARED ${geant_source_files})
target_link_libraries(GeantQED ${Geant4_LIBRARIES} Tools Particles Physics)
target_include_directories(GeantQED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
       				           ${Geant4_INCLUDE_DIRS})
install(TARGETS GeantQED
	LIBRARY DESTINATION lib)
install(FILES ${geant_header_files}
        DESTINATION include)
