#ifndef PARTICLEPUSHER_HH
#define PARTICLEPUSHER_HH

#include "Particle.hh"
#include "ParticleList.hh"
#include "EMField.hh"

class ParticlePusher
{
public:
    
    ParticlePusher(EMField* field, double dt);

    virtual ~ParticlePusher();
    
    virtual void PushParticle(Particle *part);

    void PushParticleList(ParticleList* partList);

protected:
    // position update function for charged particle
    ThreeVector PushPosition(double mass, const ThreeVector &momentum) const;

    // momentum update fuinction for charged particle
    virtual ThreeVector PushMomentum(double mass, double charge,
        const ThreeVector &momentum, const ThreeVector &Efield, 
        const ThreeVector &Bfield) const = 0;  

    EMField* m_field;   // Field which particles are pushed through
    double m_dt;        // Time of each step
};
#endif