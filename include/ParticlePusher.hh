#include "Particle.hh"

class ParticlePusher
{
public:
	
	ParticlePusher(double dt);

	~ParticlePusher();
	
	void PushParticle(Particle &part);

private:
	double m_dt;	// Time of each step
};