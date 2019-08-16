#ifndef G4CASCADETF_HH
#define G4CASCADETF_HH

#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Step.hh"
#include "G4VParticleChange.hh"
#include "TensorflowGraph.hh"
#include "UnitsSystem.hh"

class G4CascadeTF: public G4VDiscreteProcess
{
public:
    explicit G4CascadeTF(const G4String& name = "CascadeTF",
                         G4ProcessType type = fUserDefined);
    
    ~G4CascadeTF() override;

    G4bool IsApplicable(const G4ParticleDefinition& particle) override;

    G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
        const G4Step& aStep) override;


    G4double GetMeanFreePath(const G4Track& track, G4double,
                                     G4ForceCondition*) override;


    void LoadGraphs(const std::string& seedPath,
        const std::string& elecPath, const std::string& photPath);

    void SetTimeLag(double time) {m_timeLag = time;}

    void test();

private:
    TensorflowGraph* m_elecGraph;
    TensorflowGraph* m_posiGraph;
    TensorflowGraph* m_photGraph;
    TensorflowGraph* m_photCountGraph;

    double m_inParams[6];   // Normilising params for input electron
    double m_elecParmas[3]; // Normilising params for output electrons
    double m_photParams[3]; // Normilising params for output photons

    UnitsSystem* m_units;
    double m_timeLag;        // time taken to reach laser block. 
    bool m_pSwitch = false;  // switch determining if process occured
};
#endif