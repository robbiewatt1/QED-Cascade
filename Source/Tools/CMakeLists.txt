cmake_minimum_required(VERSION 3.5)
project(Tools C CXX)


if(BUILD_GEANT)
    add_compile_definitions(USEGEANT)
    find_package(Eigen3 REQUIRED)
endif(BUILD_GEANT)


set(tools_source_files
    Numerics.cpp
    MCTools.cpp
    UnitsSystem.cpp)
set(tools_header_files
    ThreeVector.hh
    ThreeMatrix.hh
    Numerics.hh
    MCTools.hh
    UnitsSystem.hh)

add_library(Tools SHARED  ${tools_source_files})

if(BUILD_GEANT)
    target_link_libraries(Tools Eigen3::Eigen)
endif(BUILD_GEANT)

target_include_directories(Tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Tools
    LIBRARY DESTINATION lib)
install(FILES ${tools_header_files}
        DESTINATION include)