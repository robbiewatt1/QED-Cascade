#include "G4NonLinearCompton.hh"
#include "G4Gamma.hh"
#include "G4Material.hh"
#include "GaussianEMField.hh"

#include "PlaneEMField.hh"
#include "StaticEMField.hh"
#include "ThreeVector.hh"

G4NonLinearCompton::G4NonLinearCompton(double dt, double tEnd,
									   const G4String& name, G4ProcessType type):
G4VDiscreteProcess(name, type), m_process(NULL), m_pusher(NULL)
{
	m_units = new UnitsSystem("SI");
	m_dt = dt / m_units->RefTime();
	m_tEnd = tEnd / m_units->RefTime();
	SetProcessSubType(1);
}

G4NonLinearCompton::~G4NonLinearCompton()
{
	delete m_field;
	delete m_part;
	delete m_g4_part;
	delete m_process;
	delete m_pusher;
}

G4VParticleChange* G4NonLinearCompton::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
    const G4DynamicParticle *g4Part = aTrack.GetDynamicParticle();
    double mass = g4Part->GetMass() * m_units->G4Energy();
    double charge = g4Part->GetCharge();
    double energy = aTrack.GetKineticEnergy() * m_units->G4Energy();
    G4ThreeVector g4_position = aTrack.GetPosition() * m_units->G4Length();
    G4ThreeVector g4_direction = aTrack.GetMomentumDirection();
    ThreeVector direction = ThreeVector(g4_direction[0],
    									g4_direction[1],
    									g4_direction[2]);
    ThreeVector position = ThreeVector(g4_position[0],
    								   g4_position[1],
    								   g4_position[2]);

	m_part = new Particle(mass, charge, energy, position, direction);
	m_photonList = new ParticleList("Photons");

	double time(0);
	while(time < m_tEnd)
	{
		m_pusher->PushParticle(*m_part);
		m_process->Interact(*m_part, m_photonList);
		time += m_dt;
	}

	aParticleChange.Initialize(aTrack);
	aParticleChange.SetNumberOfSecondaries(m_photonList->GetNPart());
	for (unsigned int i = 0; i < m_photonList->GetNPart(); i++)
	{
		Particle gamma = m_photonList->GetParticle(i);
		G4double gammaEnergy = gamma.GetEnergy() / m_units->G4Energy();
		G4ThreeVector gammaDirection = G4ThreeVector(gamma.GetDirection()[0],
					 								 gamma.GetDirection()[1],
					 								 gamma.GetDirection()[2]);
		m_g4_part = new G4DynamicParticle(G4Gamma::Gamma(), gammaDirection, gammaEnergy);
		aParticleChange.AddSecondary(m_g4_part);
	}
	G4ThreeVector partDirection = G4ThreeVector(m_part->GetDirection()[0],
					 							m_part->GetDirection()[1],
					 							m_part->GetDirection()[2]);
	aParticleChange.ProposeMomentumDirection(partDirection);
	aParticleChange.ProposeEnergy(m_part->GetEnergy() / m_units->G4Energy());

	return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

G4double G4NonLinearCompton::GetMeanFreePath(const G4Track& track, G4double,
											 G4ForceCondition*)
{
	// check if inside laser field
	G4Material* material = track.GetMaterial();
	double inField = material->GetMaterialPropertiesTable()->GetConstProperty("Laser Field");
	if (inField < 1.0)
	{
		return 1e99;
	}
	if (m_pSwitch == false)
	{
		m_pSwitch = true;
		return 1e-99;
	} else
	{
		m_pSwitch = false;
		return 1e99;
	}
}

void G4NonLinearCompton::SetStaticField(double* EField, double* BField)
{
	m_field = new StaticEMField(ThreeVector(EField[0], EField[1], EField[2]),
							    ThreeVector(BField[0], BField[1], BField[2]));
	m_process = new NonLinearCompton(m_field, m_dt);
	m_pusher  = new ParticlePusher(m_field, m_dt);
}

void G4NonLinearCompton::SetPlaneField(double maxE, double wavelength, double polerisation,
									   double* direction)
{
	m_field = new PlaneEMField(maxE / m_units->RefEField(), 
							   wavelength / m_units->RefLength(),
							   polerisation,
							   ThreeVector(direction[0], direction[1], direction[2]));	
	m_process = new NonLinearCompton(m_field, m_dt);
	m_pusher  = new ParticlePusher(m_field, m_dt);
}

void G4NonLinearCompton::SetGaussianField(double maxE, double wavelength, double tau,
										  double waist, double polerisation, double* start,
										  double* focus)
{
	m_field = new GaussianEMField(maxE / m_units->RefEField(), 
							      wavelength / m_units->RefLength(),
							      tau / m_units->RefTime(),
								  waist / m_units->RefLength(),
								  polerisation,
							  	  ThreeVector(start[0], start[1], start[2]),
							  	  ThreeVector(focus[0], focus[1], focus[2]));	
	m_process = new NonLinearCompton(m_field, m_dt);
	m_pusher  = new ParticlePusher(m_field, m_dt);
}