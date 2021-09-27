#include <cmath>
#include <fstream>

#include "ContinuousEmission.hh"
#include "Photon.hh"
#include "Numerics.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"


ContinuousEmission::ContinuousEmission(EMField* field, double dt,
    bool classical, double sampleFrac, double eMin, bool track):
PhotonEmission(field, dt, sampleFrac, eMin, track), m_classical(classical)
{
}

ContinuousEmission::~ContinuousEmission()
{
}

void ContinuousEmission::Interact(Particle *part, ParticleList *partList) const
{
    // Check for alive lepton
    if (part->GetMass() == 0 || part->IsAlive() == false) return;

    // prevent from occuring every time step
    if (MCTools::RandDouble(0, 1) > m_sampleFrac) return;

    double eta = CalculateEta(part);

    // Check for very small values of eta / classical
    double logh, chi;
    if (eta < 1.0e-12 || m_classical)
    {
        logh = 0.7193;
        double chiMin = std::exp(m_h_etaAxis[0]);
        // Extrapolate table for classical (stolen from Chris Arran).
        chi = CalculateChi(chiMin) * (eta / chiMin)  * (eta / chiMin);
    } else
    {

        logh = Numerics::Interpolate1D(m_h_etaAxis, m_h_dataTable,
            m_h_length, std::log10(eta));
        chi = CalculateChi(eta);
    }

    // Calculate photon weight
    double deltaOD = m_dt * std::sqrt(3) * UnitsSystem::alpha * eta
        * std::pow(10.0, logh)
        / (part->GetGamma() * 2.0 * UnitsSystem::pi);
    double weight = part->GetWeight() * deltaOD / m_sampleFrac;

    // Calculate photon energy
    double gammaE = 2.0 * chi * part->GetGamma() / eta;
    ThreeVector gammaP = gammaE * part->GetDirection();

    // Add new partles to the simulation
    if (gammaE > m_eMin)
    {
        Photon* photon = new Photon(gammaE, part->GetPosition(), 
            part->GetDirection(), weight, part->GetTime(), m_track);
        partList->AddParticle(photon);
    }
}