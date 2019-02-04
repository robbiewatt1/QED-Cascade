cmake_minimum_required(VERSION 3.5)
project(IO C CXX)

find_package(HDF5 REQUIRED COMPONENTS C CXX)

set(io_source_files
    Output/HDF5Output.cpp
    Output/OutputManager.cpp
    Output/Histogram.cpp
    Input/ini.cpp
    Input/INIReader.cpp
    Input/FileParser.cpp)
set(io_header_files
    Output/HDF5Output.hh
    Output/OutputManager.hh
    Output/Histogram.hh
    Input/ini.hh
    Input/INIReader.hh
    Input/FileParser.hh)

add_library(IO SHARED ${io_source_files})
target_link_libraries(IO ${HDF5_LIBRARIES} Tools Particles Physics)
target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Output
				     ${CMAKE_CURRENT_SOURCE_DIR}/Input
       				     ${HDF5_INCLUDE_DIRS})

install(TARGETS IO
	LIBRARY DESTINATION lib)
install(FILES ${io_header_files}
        DESTINATION include)
