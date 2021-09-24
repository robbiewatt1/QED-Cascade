#include <cmath>
#include <fstream>

#include "ContinuousEmission.hh"
#include "Photon.hh"
#include "Numerics.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"


ContinuousEmission::ContinuousEmission(EMField* field, double dt,
    bool classical, bool track, double eMin):
StochasticEmission(field, dt, track, eMin), m_classical(classical)
{
}

ContinuousEmission::~ContinuousEmission()
{
}

void ContinuousEmission::Interact(Particle *part, ParticleList *partList) const
{
    if (part->GetMass() == 0 || part->IsAlive() == false) return;
    // First we need to update the optical depth of the particle based on local values
    // Still need to decide of units and constants here
    double eta = CalculateEta(part);

    // Check for very small values of eta and skip interpolation
    double logh;
    if (eta < 1.0e-12)
    {
        logh = 5.24;
    } else
    {
        logh = Numerics::Interpolate1D(m_h_etaAxis, m_h_dataTable,
            m_h_length, std::log10(eta));
    }
    double deltaOD = m_dt * std::sqrt(3) * UnitsSystem::alpha * eta
        * std::pow(10.0, logh)
        / (part->GetGamma() * 2.0 * UnitsSystem::pi);
    part->UpdateOpticalDepth(deltaOD);
    // Now check if process hass occured. If so then emmit and react
    if (part->GetOpticalDepth() < 0.0)
    {
        double chi = CalculateChi(eta);
        double gammaE = 2.0 * chi * part->GetGamma() / eta;
        ThreeVector gammaP = gammaE * part->GetDirection();
        part->UpdateTrack(part->GetPosition(), part->GetMomentum() - gammaP);
        // Add new partles to the simulation
        if (gammaE > m_eMin)
        {
            Photon* photon = new Photon(gammaE, part->GetPosition(), 
                part->GetDirection(), part->GetTime(), m_track);
            partList->AddParticle(photon);
        }
        part->InitOpticalDepth();
    }
}