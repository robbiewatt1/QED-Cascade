#ifndef LORENTZPUSHER
#define LORENTZPUSHER

#include "ParticlePusher.hh"

class LorentzPusher: public ParticlePusher
{
public:
    LorentzPusher(EMField* field, double dt);

protected:
    ThreeVector PushMomentum(double mass, double charge, const ThreeVector &momentum,
            const ThreeVector &Efield, const ThreeVector &Bfield) const override;
};
#endif