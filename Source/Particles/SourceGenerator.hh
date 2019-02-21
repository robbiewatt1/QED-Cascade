#ifndef SOURCEGENERATOR_HH
#define SOURCEGENERATOR_HH

#include <vector>
#include "ThreeVector.hh"
#include "ParticleList.hh"

namespace SourceGenerator
{
	std::vector<ParticleList*> 	MonoSource(std::string type, unsigned int nPart, 
										   double energy, double deltaPos,
										   const ThreeVector &position,
										   const ThreeVector &direction);

	std::vector<ParticleList*> LinearSource(std::string type, unsigned int nPart, 
											double energyMin, double energyMax,
											double deltaPos, const ThreeVector &position, 
					  						const ThreeVector &direction);

	void FreeSources(std::vector<ParticleList*> source);
};
#endif