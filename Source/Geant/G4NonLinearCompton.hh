#ifndef G4NONLINEARCOMPTON_HH
#define G4NONLINEARCOMPTON_HH

#include "G4VDiscreteProcess.hh"
class G4NonLinearCompton: public G4VDiscreteProcess
{
public:
	explicit G4NonLinearCompton(const G4String& name = "NonLinearCompton",
								G4ProcessType type = fElectromagnetic);

	~G4NonLinearCompton() override;

	G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) override;

	G4double GetMeanFreePath(const G4Track& aTrack, G4double previousStepSize,
                             		 G4ForceCondition* condition) override;	
};
#endif