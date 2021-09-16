#ifndef RunManager_hh
#define RunManager_hh

#include "ParticlePusher.hh"
#include "ParticleList.hh"
#include "UnitsSystem.hh"
#include "Process.hh"
#include "SourceGenerator.hh"
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

class RunManager
{
public:
    RunManager();

    ~RunManager();

    // Set the time-step and end time
    void setTime(double timeStep, double timeEnd);

    // Set the field paramters
    void setField(std::string fieldType, double maxField, double wavelength, 
        double duration, double waist, double polerisation,
        const ThreeVector& start, const ThreeVector& focus);

    // Set the particle source paramters
    void setGenerator(int events, std::string particleType, 
        std::string energyDist, double energyParam1, double energyParam2,
        double radius, double duration, double divergence, 
        const ThreeVector& position, const ThreeVector& direction);

    // Define the particle pusher (Lorentz / Landau / modified landau)
    void setPusher(std::string pusherType);

    // Set if nonlinear Compton or nonlinear Breit-Wheeler are used
    void setPhysics(bool NLC, bool NLBW);

    // Simulates events
    void beamOn(int threads = 1);

    // Return input from simulation
    py::array_t<double> getInput();

    // Return output electrons from simulation
    py::array_t<double> getElectrons();

    // Return output positrons from simulation
    py::array_t<double> getPositrons();

    // Return output photons from simulation
    py::array_t<double> getPhotons();

private:

    UnitsSystem* m_units;
    EMField* m_field;
    SourceGenerator* m_generator;
    ParticlePusher* m_pusher;
    std::vector<Process*> m_processList;
    std::vector<std::vector<double>> m_input_P_X;
    std::vector<std::vector<double>> m_electron_P_X;
    std::vector<std::vector<double>> m_positron_P_X;
    std::vector<std::vector<double>> m_photon_P_X;
    double m_timeStep;
    double m_timeEnd;
};
#endif