#include "NonLinearCompton.hh"

NonLinearCompton::NonLinearCompton(LaserField* field, double dt):
m_filed(field), m_dt(dt)
{
}

NonLinearCompton::~NonLinearCompton()
{
}

void NonLinearCompton::Interact(Particle &part, ParticleList *partList)
{
	// Do some stuff
	part.UpdateOpticalDepth(0.02);
	if (part.GetOpticalDepth() < 0.0)
	{

		Particle gamma = Particle(0.0, 0.0, part.GetPosition(), part.GetMomentum(), part.GetTime(), true);
		partList->AddParticle(gamma);
		part.InitOpticalDepth();
	}
}