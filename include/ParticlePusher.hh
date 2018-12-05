#ifndef PARTICLEPUSHER_HH
#define PARTICLEPUSHER_HH

#include "Particle.hh"
#include "LaserField.hh"

class ParticlePusher
{
public:
	
	ParticlePusher(LaserField* field, double dt);

	~ParticlePusher();
	
	void PushParticle(Particle &part);

	double GetTime() const {return time;}

private:

	double PushPosition(double mass, double P);	// position update function for charged particle

	double PushMomentum(double mass, double charge, double gamma, double P1, double P2,
						double E, double B1, double B2);	// momentum update fuinction for charged particle

	LaserField* m_field;	// Field which particles are pushed through
	double m_dt;	// Time of each step
	double m_time;	// global time of simulation
};
#endif