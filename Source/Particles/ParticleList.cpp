#include "ParticleList.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"
#include <fstream>

ParticleList::ParticleList(std::string name, unsigned int maxParticles):
m_name(name), m_maxParticles(maxParticles), m_particleNumber(0)
{
	m_particleList = std::vector<Particle>(m_maxParticles);
}

ParticleList::~ParticleList()
{
}

void ParticleList::AddParticle(const Particle &part)
{
	m_particleList[m_particleNumber] = part;
	m_particleNumber++;
}

void ParticleList::GenericSource(unsigned int nPart, double mass, double charge, double energy,
							 	 double deltaPos, const ThreeVector &position,
							 	 const ThreeVector &direction)
{
	m_particleNumber = nPart;
	std::vector<double> rPos = MCTools::SampleNorm(0, deltaPos, nPart);
	ThreeMatrix m_rotaion = direction.RotateToAxis(ThreeVector(0, 0, 1));
	for (unsigned int i = 0; i < nPart; i++)
	{
		// This method samples from circle at origin, then rotates to surface normal and
		// adds to position to generate source at arb location and direction 
		double angle = MCTools::RandDouble(0, 2.0 * UnitsSystem::pi);
		ThreeVector partPosition = ThreeVector(std::cos(angle) * rPos[i], 
											   std::sin(angle) * rPos[i], 0.0);
		partPosition = m_rotaion * partPosition + position;
		double momMag = std::sqrt(energy * energy - mass * mass);
		
		Particle part = Particle(mass, charge, partPosition, momMag * direction.Norm());
		m_particleList[i] = part;		
	}
}

