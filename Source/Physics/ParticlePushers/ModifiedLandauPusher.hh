#ifndef ModifiedLandauPusher_HH
#define ModifiedLandauPusher_HH

#include "ParticlePusher.hh"

class ModifiedLandauPusher: public ParticlePusher
{
public:
    ModifiedLandauPusher(EMField* field, double dt);
 

protected:
    ThreeVector PushMomentum(double mass, double charge,
        const ThreeVector &momentum, const ThreeVector &Efield,
        const ThreeVector &Bfield) const override;

private:
    double gaunt(double eta) const;
};
#endif