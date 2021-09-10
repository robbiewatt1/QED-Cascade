find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(QEDCascPy SHARED  RunManager.cpp QEDCascPy-Bindings.cpp)
target_link_libraries(QEDCascPy PUBLIC Tools Particles PhysicsQED)