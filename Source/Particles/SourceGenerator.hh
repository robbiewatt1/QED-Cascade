#ifndef SOURCEGENERATOR_HH
#define SOURCEGENERATOR_HH

#include <vector>
#include "ThreeVector.hh"
#include "ParticleList.hh"

class SourceGenerator
 {
 public:
    SourceGenerator(std::string type, std::string distro, 
                    unsigned int nPart, double energyMin,
                    double energyMax, double deltaPos, 
                    double deltaTau,
                    const ThreeVector &position,
                    const ThreeVector &direction,
                    bool track = false);
    
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
    std::vector<double> m_longPos;
    std::vector<double> m_angle;
    std::vector<double> m_energy;
    unsigned int m_partCount;
    ThreeMatrix m_rotaion;
    bool m_track;
};
#endif