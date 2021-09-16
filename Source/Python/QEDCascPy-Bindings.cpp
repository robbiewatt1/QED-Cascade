#include "RunManager.hh"
#include "ThreeVector.hh"
#include <pybind11/pybind11.h>

namespace py = pybind11;


PYBIND11_MODULE(QEDCascPy, module)
{
    py::class_<RunManager>(module, "RunManager")
        .def(py::init<>())
        .def("setTime", &RunManager::setTime, "Set the end time and time-step")
        .def("setField", &RunManager::setField, "Set the field properties")
        .def("setGenerator", &RunManager::setGenerator, "Set the particle source")
        .def("setPusher", &RunManager::setPusher, "Set the particle pusher")
        .def("setPhysics", &RunManager::setPhysics, "Select processes")
        .def("beamOn", &RunManager::beamOn, "Run the simulation", 
            py::arg("threads") = 1)
        .def("getInput", &RunManager::getInput,
            "Get particle properties before interaction")
        .def("getElectrons", &RunManager::getElectrons,
            "Get electron properties before interaction")
        .def("getPositrons", &RunManager::getPositrons,
            "Get electron properties before interaction")
        .def("getPhotons", &RunManager::getPhotons,
            "Get electron properties before interaction");

    py::class_<ThreeVector>(module, "ThreeVector")
        .def(py::init<double, double, double>());
}
