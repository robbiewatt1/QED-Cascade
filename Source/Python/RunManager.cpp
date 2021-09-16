#include "RunManager.hh"

#include "GaussianEMField.hh"
#include "FocusingField.hh"
#include "NonLinearCompton.hh"
#include "NonLinearBreitWheeler.hh"
#include "LorentzPusher.hh"
#include "LandauPusher.hh"
#include "ModifiedLandauPusher.hh"
#include <pybind11/stl.h>


#include <omp.h>




RunManager::RunManager():
m_timeStep(0), m_timeEnd(0)
{
    // Update this if I ever add another unit system
    m_units = new UnitsSystem("SI");
}

RunManager::~RunManager()
{
    delete m_units;
    delete m_field;
    delete m_generator;
    delete m_pusher;
    m_processList.clear();
}

void RunManager::setTime(double timeStep, double timeEnd)
{
    m_timeStep = timeStep / m_units->RefTime();
    m_timeEnd = timeEnd / m_units->RefTime();
}

void RunManager::setField(std::string fieldType, double maxField, 
    double wavelength, double duration, double waist, double polerisation,
    const ThreeVector& start, const ThreeVector& focus)
{
    if (fieldType == "gaussian" || fieldType == "Gaussian")
    {
        m_field = new GaussianEMField(maxField / m_units->RefEField(),
                                    wavelength / m_units->RefLength(),
                                    duration / m_units->RefTime(),
                                    waist / m_units->RefLength(),
                                    polerisation,
                                    start / m_units->RefLength(),
                                    focus / m_units->RefLength());
    } else if (fieldType == "focusing" || fieldType == "Focusing")
    {
        m_field = new FocusingField(maxField / m_units->RefEField(),
                                    wavelength / m_units->RefLength(),
                                    duration / m_units->RefTime(),
                                    waist / m_units->RefLength(),
                                    polerisation,
                                    start / m_units->RefLength(),
                                    focus / m_units->RefLength());
    } else
    {
        std::cerr << "Error: Unknown field type." << std::endl;
        return;
    }
}

void RunManager::setPusher(std::string pusherType)
{
    // Check that other stuff is set first

    if (pusherType == "Lorentz")
    {
        m_pusher = new LorentzPusher(m_field, m_timeStep);
    } else if (pusherType == "Landau")
    {
        m_pusher = new LandauPusher(m_field, m_timeStep);
    } else if (pusherType == "ModifiedLandau")
    {
        m_pusher = new ModifiedLandauPusher(m_field, m_timeStep);
    } else
    {
        std::cerr << "Error: Unkown Pusher Type" << std::endl;
        return;
    }
}

void RunManager::setGenerator(int events, std::string particleType,
    std::string energyDist, double energyParam1, double energyParam2,
    double radius, double duration, double divergence, 
    const ThreeVector& position, const ThreeVector& direction)
{
    m_generator = new SourceGenerator(particleType, 
                                      energyDist,
                                      events,
                                      energyParam1 / m_units->RefEnergy(),
                                      energyParam2 / m_units->RefEnergy(),
                                      radius / m_units->RefLength(),
                                      duration / m_units->RefTime(),
                                      divergence,
                                      position / m_units->RefLength(),
                                      direction);
}

void RunManager::setPhysics(bool NLC, bool NLBW)
{

    // Needs check for timeStep first

    if (NLC == true)
    {
        NonLinearCompton* compton = new NonLinearCompton(m_field, m_timeStep,
                false, 0);
        m_processList.push_back(compton);
    }
    if (NLBW == true)
    {
        NonLinearBreitWheeler* breitWheeler = new NonLinearBreitWheeler(m_field, 
            m_timeStep, false);
        m_processList.push_back(breitWheeler);
    }
}


void RunManager::beamOn(int threads)
{

    m_input_P_X = std::vector<std::vector<double>>(threads);
    m_electron_P_X = std::vector<std::vector<double>>(threads);
    m_positron_P_X = std::vector<std::vector<double>>(threads);
    m_photon_P_X = std::vector<std::vector<double>>(threads);

    long unsigned int nEvents = m_generator->GetSourceNumber();
    // Simulate events
    omp_set_num_threads(threads);
    #pragma omp parallel for
    for (unsigned int i = 0; i < nEvents; i++) //loop primes
    {
        int tid = omp_get_thread_num();
        ParticleList* event = m_generator->GenerateList();

        // Store inital particle properties
        for (unsigned int j = 0; j < event->GetNPart(); j++) // Loop particles
        {
            m_input_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[0]
                * m_units->RefMomentum());
            m_input_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[1]
                * m_units->RefMomentum());
            m_input_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[2]
                * m_units->RefMomentum());
            m_input_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[0]
                * m_units->RefLength());
            m_input_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[1]
                * m_units->RefLength());
            m_input_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[2]
                * m_units->RefLength());
        }

        double time(0);
        while(time < m_timeEnd) //loop time
        {
            // Push particles and interact
            for (unsigned int j = 0; j < event->GetNPart(); j++) // Loop particles
            {
                m_pusher->PushParticle(event->GetParticle(j));
                for (unsigned int proc = 0; proc < m_processList.size(); proc++) // loop processes
                {
                    m_processList[proc]->Interact(event->GetParticle(j), event);
                }
            }
            time += m_timeStep;
        }
        // Store final particle properties
        for (long unsigned int j = 0; j < event->GetNPart(); ++j)
        {
            if (event->GetParticle(j)->GetCharge() == -1)
            {
                m_electron_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[0] 
                    * m_units->RefMomentum());
                m_electron_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[1]
                    * m_units->RefMomentum());
                m_electron_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[2]
                    * m_units->RefMomentum());
                m_electron_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[0]
                    * m_units->RefLength());
                m_electron_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[1]
                    * m_units->RefLength());
                m_electron_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[2]
                    * m_units->RefLength());
            } else if (event->GetParticle(j)->GetCharge() == 0)
            {
                m_photon_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[0]
                    * m_units->RefMomentum());
                m_photon_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[1]
                    * m_units->RefMomentum());
                m_photon_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[2]
                    * m_units->RefMomentum());
                m_photon_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[0]
                    * m_units->RefLength());
                m_photon_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[1]
                    * m_units->RefLength());
                m_photon_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[2]
                    * m_units->RefLength());
            } else if (event->GetParticle(j)->GetCharge() == 1)
            {
                m_positron_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[0]
                    * m_units->RefMomentum());
                m_positron_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[1]
                    * m_units->RefMomentum());
                m_positron_P_X[tid].push_back(event->GetParticle(j)->GetMomentum()[2]
                    * m_units->RefMomentum());
                m_positron_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[0]
                    * m_units->RefLength());
                m_positron_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[1]
                    * m_units->RefLength());
                m_positron_P_X[tid].push_back(event->GetParticle(j)->GetPosition()[2]
                    * m_units->RefLength());
            }
        }
        m_generator->FreeSources(event);
    }
}

py::array_t<double> RunManager::getInput()
{
    std::vector<double> accum;
    for (auto& sub : m_input_P_X)
    {
        accum.insert(accum.end(), sub.begin(), sub.end());
    }
    int particles = accum.size() / 6;
    py::array_t<double> result = py::cast(accum);
    result.resize({particles, 6});
    return result;
}

py::array_t<double> RunManager::getElectrons()
{
    std::vector<double> accum;
    for (auto& sub : m_electron_P_X)
    {
        accum.insert(accum.end(), sub.begin(), sub.end());
    }
    int particles = accum.size() / 6;
    py::array_t<double> result = py::cast(accum);
    result.resize({particles, 6});
    return result;
}

py::array_t<double> RunManager::getPositrons()
{
    std::vector<double> accum;
    for (auto& sub : m_positron_P_X)
    {
        accum.insert(accum.end(), sub.begin(), sub.end());
    }
    int particles = accum.size() / 6;
    py::array_t<double> result = py::cast(accum);
    result.resize({particles, 6});
    return result;
}

py::array_t<double> RunManager::getPhotons()
{
    std::vector<double> accum;
    for (auto& sub : m_photon_P_X)
    {
        accum.insert(accum.end(), sub.begin(), sub.end());
    }
    int particles = accum.size() / 6;
    py::array_t<double> result = py::cast(accum);
    result.resize({particles, 6});
    return result;
}