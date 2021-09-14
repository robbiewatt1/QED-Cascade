if(SKBUILD)
  # Scikit-Build does not add your site-packages to the search path automatically,
  # so we need to add it _or_ the pybind11 specific directory here.
  execute_process(
    COMMAND
      "${PYTHON_EXECUTABLE}" -c
      "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE _tmp_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
  list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
endif()
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(QEDCascPy MODULE  RunManager.cpp QEDCascPy-Bindings.cpp)
target_link_libraries(QEDCascPy PRIVATE Tools Particles PhysicsQED)


if(APPLE)
  set(_rpath_value "@loader_path")
elseif(UNIX)
  set(_rpath_value "$ORIGIN")
endif()
message(${_rpath_value})
set_target_properties(QEDCascPy PROPERTIES
                        INSTALL_RPATH ${_rpath_value}/lib)

install(TARGETS QEDCascPy DESTINATION .)