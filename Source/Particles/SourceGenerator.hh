#ifndef SOURCEGENERATOR_HH
#define SOURCEGENERATOR_HH

#include <vector>
#include "ThreeVector.hh"
#include "ParticleList.hh"

class SourceGenerator
 {
 public:
 	SourceGenerator(std::string type, unsigned int nPart, 
					double energyMin, double energyMax, 
					double deltaPos, const ThreeVector &position,
					const ThreeVector &direction);
 	
 	~SourceGenerator();

 	ParticleList* GenerateList();

 	void FreeSources(ParticleList* source);

 	unsigned int GetSourceNumber() const {return m_nPart;} 

private:

	std::string m_type;
	unsigned int m_nPart;
	ThreeVector m_position;
	ThreeVector m_direction;
	std::vector<double> m_radialPos;
	std::vector<double> m_angle;
	std::vector<double> m_energy;
	unsigned int m_partCount;
	ThreeMatrix m_rotaion;
};
#endif