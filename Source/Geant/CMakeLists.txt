cmake_minimum_required(VERSION 3.5)
project(GeantQED C CXX)
find_package(Geant4 REQUIRED)

set(geant_source_files
    G4NonLinearCompton.cpp)
set(geant_header_files
    G4NonLinearCompton.hh)

if(BUILD_TENSORFLOW)
	find_package(TensorflowCC REQUIRED COMPONENTS Shared)
	set(tf_header Tensorflow/TensorflowGraph.hh)
	set(tf_source Tensorflow/TensorflowGraph.cpp)
endif()

add_library(GeantQED SHARED ${geant_source_files} ${tf_source})
include(${Geant4_USE_FILE})
message(${Geant4_USE_FILE})
target_link_libraries(GeantQED ${Geant4_LIBRARIES} TensorflowCC::Shared Tools Particles PhysicsQED)
target_include_directories(GeantQED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
       				           ${Geant4_INCLUDE_DIRS})
install(TARGETS GeantQED
	LIBRARY DESTINATION lib)
install(FILES ${geant_header_files} ${tf_header}
        DESTINATION include)


message(${geant_source_files})
