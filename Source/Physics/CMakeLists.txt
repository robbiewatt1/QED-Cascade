set(physics_source_files
	Fields/EMField/GaussianEMField.cpp
	Fields/EMField/StaticEMField.cpp
	Fields/EMField/PlaneEMField.cpp
        Fields/EMField/FocusingField.cpp
        Processes/PhotonEmission.cpp
        Processes/ContinuousEmission.cpp
	Processes/StochasticEmission.cpp
	Processes/NonLinearBreitWheeler.cpp
	ParticlePushers/ParticlePusher.cpp
        ParticlePushers/ParticlePusher.cpp
        ParticlePushers/LandauPusher.cpp
        ParticlePushers/ModifiedLandauPusher.cpp
        ParticlePushers/LorentzPusher.cpp)
set(physics_header_files
	Fields/EMField/EMField.hh
	Fields/EMField/GaussianEMField.hh
        Fields/EMField/StaticEMField.hh
        Fields/EMField/PlaneEMField.hh
        Fields/EMField/FocusingField.hh
        Processes/Process.hh
	Processes/PhotonEmission.hh
        Processes/ContinuousEmission.hh
        Processes/StochasticEmission.hh
	Processes/NonLinearBreitWheeler.hh
        ParticlePushers/ParticlePusher.hh
        ParticlePushers/LandauPusher.hh
        ParticlePushers/ModifiedLandauPusher.hh
        ParticlePushers/LorentzPusher.hh)
set(physics_table_files
    ../../Tables/chimin.table
    ../../Tables/e_split.table
    ../../Tables/energy_split.table
    ../../Tables/epsilon.table
    ../../Tables/hsokolov.table
    ../../Tables/ksi_sokolov.table
    ../../Tables/log_chi.table
    ../../Tables/pairprod.table)


add_library(PhysicsQED SHARED ${physics_source_files})
target_link_libraries(PhysicsQED Tools Particles)
target_include_directories(PhysicsQED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Fields/EMField 
					  ${CMAKE_CURRENT_SOURCE_DIR}/Processes
					  ${CMAKE_CURRENT_SOURCE_DIR}/ParticlePushers)

install(TARGETS PhysicsQED
    	LIBRARY DESTINATION lib)
install(FILES ${physics_header_files}
        DESTINATION include)
install(FILES ${physics_table_files}
        DESTINATION tables)