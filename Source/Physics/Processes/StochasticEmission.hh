#ifndef StochasticEmission_HH
#define StochasticEmission_HH

#include "PhotonEmission.hh"

class StochasticEmission: public PhotonEmission
{
public:
    StochasticEmission(EMField* field, double dt, double sampleFrac = 1,
        double eMin = 0, bool track = false);

    virtual ~StochasticEmission();

    void Interact(Particle *part, ParticleList *partList) const override;
    
};
#endif