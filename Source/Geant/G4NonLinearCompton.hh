#ifndef G4NONLINEARCOMPTON_HH
#define G4NONLINEARCOMPTON_HH

#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Step.hh"
#include "G4VParticleChange.hh"

#include "Particle.hh"
#include "ParticleList.hh"
#include "LorentzPusher.hh"
#include "NonLinearCompton.hh"

class G4NonLinearCompton: public G4VDiscreteProcess
{
public:
	explicit G4NonLinearCompton(double dt, double tEnd,
								const G4String& name = "NonLinearCompton",
								G4ProcessType type = fUserDefined);

	~G4NonLinearCompton() override;

	G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) override;

	G4double GetMeanFreePath(const G4Track& track, G4double,
                             		 G4ForceCondition*) override;

	// Set a constant with E = field[0-2] B = field [3-5]
	void SetStaticField(double* EField, double* BField);

	// Sets a plane laser field
	void SetPlaneField(double maxE, double wavelength, double polerisation, double* direction);

	// sets a gaussian laser pulse
	void SetGaussianField(double maxE, double wavelength, double tau, double waist,
						  double polerisation, double* start, double* focus);

private:
	double m_dt;					// Time step of process
	double m_tEnd;					// max time of 
	bool m_pSwitch = false;			// switch determining if process occured

	UnitsSystem* m_units;			// QED units system
	Particle* m_part;				// qed particle 
	ParticleList* m_photonList;		// list of secondaries
	EMField* m_field;				// field which particles are pushed through
	NonLinearCompton* m_process;	// non linear compton process
	LorentzPusher* m_pusher;		// particle poshuer
	
	G4DynamicParticle* m_g4_part;	// g4 particle
};
#endif