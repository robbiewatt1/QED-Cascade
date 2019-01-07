#include "ParticleList.hh"
#include "MCTools.hh"
#include "Constants.hh"
#include <fstream>

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

void ParticleList::GenericSource(unsigned int nPart, double mass, double charge, double energy,
							 	 double deltaPos, const ThreeVector &position,
							 	 const ThreeVector &direction)
{
	m_particleList = std::vector<Particle>(nPart);
	std::vector<double> rPos = MCTools::SampleNorm(0, deltaPos, nPart);
	ThreeMatrix m_rotaion = direction.RotateToAxis(ThreeVector(0, 0, 1));
	for (unsigned int i = 0; i < nPart; i++)
	{
		// This method samples from circle at origin, then rotates to surface normal and
		// adds to position to generate source at arb location and direction 
		double angle = MCTools::RandDouble(0, 2.0 * Constants::pi);
		ThreeVector partPosition = ThreeVector(std::cos(angle) * rPos[i], 
											   std::sin(angle) * rPos[i], 0.0);
		partPosition = m_rotaion * partPosition + position;
		double momMag = std::sqrt(energy * energy - mass * mass);
		
		Particle part = Particle(mass, charge, partPosition, momMag * direction.Norm(), 0, true);
		m_particleList[i] = part;		
	}
}

void ParticleList::SaveTracks(HDF5Output *file) const
{
	file->AddGroup("Particles");
	for (unsigned int i = 0; i < m_particleList.size(); i++)
	{
		m_particleList[i].SaveTrack(file, i);
	}
}