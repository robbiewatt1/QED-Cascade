#ifndef NONLINEARBREITWHEELER_HH
#define NONLINEARBREITWHEELER_HH

#include "Process.hh"
#include "Photon.hh"

class NonLinearBreitWheeler: public Process 
{
public:
    NonLinearBreitWheeler(EMField* field, double dt, bool track = false);

    virtual ~NonLinearBreitWheeler();

    void Interact(Particle* part, ParticleList *partList) const override;

private:

    double CalculateChi(Particle* part) const;

    double CalculateSplit(double chi) const;

    void LoadTables();

    void UnloadTables();

private:
    // Data for t tables
    double* m_t_dataTable;
    double* m_t_chiAxis;
    unsigned int m_t_length;

    double** m_eFract_dataTable;
    double* m_eFract_chiAxis;
    double* m_eFract_fractAxis;
    unsigned int m_efract_length;
};
#endif