#ifndef ContinuousEmission_HH
#define ContinuousEmission_HH

#include "PhotonEmission.hh"

class ContinuousEmission: public PhotonEmission
{
public:
    ContinuousEmission(EMField* field, double dt, bool classical = false, 
        bool track = false, double eMin = 0);
    
    virtual ~ContinuousEmission();

    void Interact(Particle* part, ParticleList *partList) const override;

private:
    bool m_classical;
};
#endif