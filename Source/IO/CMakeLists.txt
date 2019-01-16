cmake_minimum_required(VERSION 3.5)
project(IO C CXX)

find_package(HDF5 REQUIRED COMPONENTS C CXX)

set(io_source_files
    Output/HDF5Output.cpp
    Output/OutputManager.cpp
    Input/ini.cpp
    Input/INIReader.cpp)
add_library(IO SHARED ${io_source_files})
target_link_libraries(IO ${HDF5_LIBRARIES} Tools Particles Physics)
target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Output
				     ${CMAKE_CURRENT_SOURCE_DIR}/Input
       				     ${HDF5_INCLUDE_DIRS})

install(TARGETS IO
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
