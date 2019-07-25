cmake_minimum_required(VERSION 3.5)
project(GeantQED C CXX)
find_package(Geant4 REQUIRED)

set(geant_source_files
    G4NonLinearCompton.cpp)
set(geant_header_files
    G4NonLinearCompton.hh)

if(BUILD_TENSORFLOW)
	find_package(TensorflowCC REQUIRED COMPONENTS Shared)
	set(tf_header_files TensorflowGraph.hh)
	set(tf_source_files TensorflowGraph.cpp)
endif()

message(${tf_header_files})

add_library(GeantQED SHARED ${geant_source_files} ${tf_source_files})
include(${Geant4_USE_FILE})
target_link_libraries(GeantQED ${Geant4_LIBRARIES} TensorflowCC::Shared Tools Particles PhysicsQED)
target_include_directories(GeantQED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
       				           ${Geant4_INCLUDE_DIRS})

install(TARGETS GeantQED
	LIBRARY DESTINATION lib)
install(FILES ${geant_header_files} ${tf_header_files}
        DESTINATION include)
