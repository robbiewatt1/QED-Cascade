#ifndef PARTICLELIST_HH
#define PARTICLELIST_HH

#include<vector>

#include "Particle.hh"
#include "ThreeVector.hh"

class ParticleList
{
public:
	ParticleList();
	
	~ParticleList();

	// Adds a particle to the source. Not very fast for large arrays
	void AddParticle(const Particle &part);

	// Generates a generic particle source
	void GenericSource(unsigned int nPart, double mass, double charge, double energy, 
					   const ThreeVector &position, const ThreeVector &direction);

	void SaveTracks(HDF5Output &file) const;

	Particle& operator[](unsigned int index){return m_particleList[index];}


private:
	std::vector<Particle> m_particleList;
};

#endif