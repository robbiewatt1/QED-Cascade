cmake_minimum_required(VERSION 3.5)
project(QEDCASC)

enable_language(CXX)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-fopenmp")

#find_package(HDF5 REQUIRED COMPONENTS C CXX)
#find_package(Boost REQUIRED COMPONENTS filesystem)
#find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(QEDCASC main.cpp ${sources} ${headers})
target_link_libraries(QEDCASC)

install(TARGETS QEDCASC DESTINATION bin)
