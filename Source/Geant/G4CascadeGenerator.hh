#ifndef G4CascadeGenerator_hh
#define G4CascadeGenerator_hh

#include <vector>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "EMField.hh"
#include "ParticlePusher.hh"
#include "ParticleList.hh"
#include "SourceGenerator.hh"
#include "UnitsSystem.hh"
#include "Process.hh"


class G4CascadeGenerator: public G4VUserPrimaryGeneratorAction
{
public:
    G4CascadeGenerator(std::string iniPath, G4ThreeVector& position);
 
    virtual ~G4CascadeGenerator();

    void SimulateEvent();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun* m_particleGun;
    G4ParticleDefinition* m_electronDef;
    G4ParticleDefinition* m_positronDef;
    G4ParticleDefinition* m_gammaDef;

    UnitsSystem* m_units;
    ParticlePusher* m_pusher;
    EMField* m_field;
    ParticleList* m_event;
    SourceGenerator* m_source;
    std::vector<Process*> m_processList;
    double m_timeStep;
    double m_timeEnd;
    unsigned int m_currentEvent;
    int m_currentSecondary;
};
#endif