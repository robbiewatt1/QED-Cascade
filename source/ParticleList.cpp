#include "ParticleList.hh"

ParticleList::ParticleList()
{
}

ParticleList::~ParticleList()
{
}

void ParticleList::AddParticle(const Particle &part)
{
	m_particleList.push_back(part);
}

void Particle::GenericSource(unsigned int nPart, double mass, double charge, double energy, 
							 double deltaPos, const ThreeVector &position,
							 const ThreeVector &direction)
{
	m_particleList = std::vector<Particle>(nPart);
	for (unsigned int i = 0; i < nPart; i++)
	{
		// sample from circle

		double momMag = std::sqrt(energy * energy - mass * mass);
		Particle part = Particle(mass, charge, , momMag * direction.Norm()) 
	}

}