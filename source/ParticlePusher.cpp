#include "ParticlePusher.hh"

ParticlePusher::ParticlePusher():
m_dt(dt)
{
}

ParticlePusher::~ParticlePusher()
{
}

void ParticlePusher::PushParticle(Particle &part)
{
	if (part.GetCharge() == 0)	// Particle is Chargless
	{
		
	}

}