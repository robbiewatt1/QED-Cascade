#ifndef PARTICLEPUSHER_HH
#define PARTICLEPUSHER_HH

#include "Particle.hh"
#include "ParticleList.hh"
#include "EMField.hh"

class ParticlePusher
{
public:
	
	ParticlePusher(EMField* field, double dt);

	~ParticlePusher();
	
	void PushParticle(Particle *part);

	void PushParticleList(ParticleList* partList);

	double GetTime() const {return m_time;}

private:
	// position update function for charged particle
	ThreeVector PushPosition(double mass, const ThreeVector &momentum);

	// momentum update fuinction for charged particle
	ThreeVector PushMomentum(double mass, double charge, const ThreeVector &momentum,
						const ThreeVector &Efield, const ThreeVector &Bfield);	

	EMField* m_field;	// Field which particles are pushed through
	double m_dt;	// Time of each step
	double m_time;	// global time of simulation
};
#endif