find_package(Geant4 REQUIRED)

set(geant_source_files
    G4CascadeGenerator.cpp
    G4StochasticEmission.cpp)
set(geant_header_files
    G4CascadeGenerator.hh
    G4StochasticEmission.hh)

if(BUILD_TENSORFLOW)
    find_package(TensorflowCC REQUIRED)
    set(tf_header_files TensorflowGraph.hh
                        G4CascadeTF.hh)
    set(tf_source_files TensorflowGraph.cpp
                        G4CascadeTF.cpp)
endif()

add_library(GeantQED SHARED ${geant_source_files} ${tf_source_files})
include(${Geant4_USE_FILE})
target_link_libraries(GeantQED ${Geant4_LIBRARIES} Tools IO Particles PhysicsQED)
if(BUILD_TENSORFLOW)
    target_link_libraries(GeantQED TensorflowCC::Shared)
endif()

target_include_directories(GeantQED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                               ${Geant4_INCLUDE_DIRS})

install(TARGETS GeantQED
    LIBRARY DESTINATION lib)
install(FILES ${geant_header_files} ${tf_header_files}
        DESTINATION include)
