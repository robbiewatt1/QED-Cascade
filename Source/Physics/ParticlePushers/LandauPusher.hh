#ifndef LANDAUPUSHER_HH
#define LANDAUPUSHER_HH

#include "ParticlePusher.hh"

class LandauPusher: public ParticlePusher
{
public:
	LandauPusher(EMField* field, double dt);
	
protected:
    ThreeVector PushMomentum(double mass, double charge, const ThreeVector &momentum,
    		const ThreeVector &Efield, const ThreeVector &Bfield) const override;	
};
#endif