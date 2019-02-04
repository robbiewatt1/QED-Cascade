#include "G4NonLinearCompton.hh"

G4NonLinearCompton::G4NonLinearCompton(const G4String& name, G4ProcessType type):
G4VDiscreteProcess(name, type)
{
}

G4NonLinearCompton::~G4NonLinearCompton()
{
}

G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
//	return G4VDiscreteProcess::PostStepDoIt();
}

G4double GetMeanFreePath(const G4Track& aTrack, G4double previousStepSize,
                             		 G4ForceCondition* condition)
{
	return 1;
}