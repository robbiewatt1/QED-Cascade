#ifndef ContinuousEmission_HH
#define ContinuousEmission_HH

#include "PhotonEmission.hh"

class ContinuousEmission: public PhotonEmission
{
public:
    ContinuousEmission(EMField* field, double dt, bool classical = false,
        double sampleFrac = 1, double eMin = 0, bool track = false);
    
    virtual ~ContinuousEmission();

    void Interact(Particle* part, ParticleList *partList) const override;

private:
    bool m_classical;
};
#endif