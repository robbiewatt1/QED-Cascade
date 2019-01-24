#ifndef PARTICLELIST_HH
#define PARTICLELIST_HH

#include<vector>
#include <string>

#include "Particle.hh"
#include "ThreeVector.hh"

class ParticleList
{
public:
	/* Default constructor. The maximum number of particles is set 
	   before to avoid appending to list */
	ParticleList(std::string name, unsigned int maxParticles = 1e7);
	
	~ParticleList();

	std::string GetName() const {return m_name;}

	unsigned int GetNPart() const {return m_particleNumber;}

	Particle& GetParticle(unsigned int index) {return m_particleList[index];}	

	// Adds a particle to the source. Not very fast for large arrays
	void AddParticle(const Particle &part);

	// Generates a generic particle source
	void GenericSource(unsigned int nPart, double mass, double charge, double energy,
					   double deltaPos, const ThreeVector &position,
					   const ThreeVector &direction);

//	void SaveTracks(HDF5Output *file, std::string partName) const;

private:
	std::string m_name;
	unsigned int m_maxParticles;	// The maximum number of particles the list can take
	unsigned int m_particleNumber;	// The current number of particles in the list
	std::vector<Particle> m_particleList;	// the list containing all the particles
};

#endif