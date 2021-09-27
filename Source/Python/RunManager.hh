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

    // Set the field parameters
    void setField(const std::string& fieldType, double maxField,
        double wavelength, double duration, double waist, double polarisation,
        const ThreeVector& start, const ThreeVector& focus);

    // Set the particle source parameters
    void setGenerator(const std::string& particleType,
        const std::string& energyDist, double energyParam1, double energyParam2,
        double radius, double duration, double divergence, 
        const ThreeVector& position, const ThreeVector& direction);

    // Set if nonlinear Compton or nonlinear Breit-Wheeler are used
    void setPhysics(const std::string& physics);

    // Set the fraction of photons that are tracked for continuous emission.
    void setSampleFraction(double frac);

    // Turn nonlinear Breit-Wheeler process on
    void usePairProduction(bool useBW);

    // Simulates events
    void beamOn(int events, int threads = 1);

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
    ParticlePusher* m_pusher;
    SourceGenerator* m_generator;
    std::vector<Process*> m_processList;
    std::vector<std::vector<double>> m_input_P_X;
    std::vector<std::vector<double>> m_electron_P_X;
    std::vector<std::vector<double>> m_positron_P_X;
    std::vector<std::vector<double>> m_photon_P_X;
    
    // Time properties
    double m_timeStep;
    double m_timeEnd;

    // Field properties
    bool m_fieldSet;
    std::string m_fieldType;
    double m_maxField;
    double m_wavelength;
    double m_fieldDuration;
    double m_waist;
    double m_polarisation;
    ThreeVector m_start;
    ThreeVector m_focus;

    // Pusher properties
    bool m_physSet;
    std::string m_physics;
    double m_sampleFrac;

    // Physics properties
    bool m_useBW;

    // Generator properties
    bool m_genSet;
    std::string m_particleType;
    std::string m_energyDist;
    double m_energyParam1; 
    double m_energyParam2;
    double m_radius;
    double m_particleDuration;
    double m_divergence;
    ThreeVector m_position;
    ThreeVector m_direction;
};
#endif