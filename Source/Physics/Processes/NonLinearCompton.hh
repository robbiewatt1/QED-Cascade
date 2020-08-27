#ifndef NONLINEARCOMPTON_HH
#define NONLINEARCOMPTON_HH

#include "Process.hh"
#include "EMField.hh"
#include "Lepton.hh"
#include "ParticleList.hh"
#include "UnitsSystem.hh"

class NonLinearCompton: public Process
{
public:
    NonLinearCompton(EMField* field, double dt, bool track = false,
        double eMin = 0);
    
   virtual ~NonLinearCompton();

    // Main function carrying out the process. The particle is the iunterafting 
    // particle and the particle list is where the new particle will be added.
    void Interact(Particle* part, ParticleList *partList) const override;

private:
    
    // Calculates the value of eta at the particles current location
    double CalculateEta(Particle* part) const;

    double CalculateH(double eta) const;

    double CalculateChi(double eta) const;  // Very bad method at the moment

    void LoadTables();

    void UnloadTables();

private:
    // Minimum energy of tracked photon
    double m_eMin;

    // Data tables for h factor
    double* m_h_dataTable;
    double* m_h_etaAxis;
    unsigned int m_h_length;

    // Data tables used for calculating the photon energy
    double** m_phEn_dataTable;
    double** m_phEn_chiAxis;
    double* m_phEn_chiMinAxis;  
    double* m_phEn_etaAxis;
    unsigned int m_phEn_etaLength, m_phEn_chiLength;
};
#endif