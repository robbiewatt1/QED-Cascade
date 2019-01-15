cmake_minimum_required(VERSION 3.5)
project(IO C CXX)

find_package(HDF5 REQUIRED COMPONENTS C CXX)

set(io_source_files
    HDF5Output.hh
    HDF5Output.cpp
    ini.hh
    ini.cpp
    INIReader.hh
    INIReader.cpp)
add_library(IO SHARED ${io_source_files})
target_link_libraries(IO ${HDF5_LIBRARIES} Tools)
target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIRS})

install(TARGETS IO
	COMPONENT application
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
