#ifndef PhotonEmission_HH
#define PhotonEmission_HH

#include "Process.hh"
#include "EMField.hh"
#include "Lepton.hh"
#include "ParticleList.hh"
#include "UnitsSystem.hh"

class PhotonEmission: public Process
{
public:
    PhotonEmission(EMField* field, double dt, double sampleFrac, 
        double eMin, bool track);
    
    virtual ~PhotonEmission();

    virtual void Interact(Particle *part, ParticleList *partList) const = 0;

protected:
    
    double CalculateEta(Particle* part) const;

    double CalculateH(double eta) const;

    double CalculateChi(double eta) const;

    void LoadTables();

    void UnloadTables();

    // Fraction of photons emitted
    double m_sampleFrac;
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