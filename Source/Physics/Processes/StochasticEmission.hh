#ifndef StochasticEmission_HH
#define StochasticEmission_HH

#include "PhotonEmission.hh"

class StochasticEmission: public PhotonEmission
{
public:
    StochasticEmission(EMField* field, double dt, bool track = false,
        double eMin = 0);

    virtual ~StochasticEmission();

    void Interact(Particle *part, ParticleList *partList) const override;
    
};
#endif