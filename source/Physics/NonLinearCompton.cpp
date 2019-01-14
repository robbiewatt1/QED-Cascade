#include <cmath>
#include <fstream>

#include "NonLinearCompton.hh"
#include "Numerics.hh"


NonLinearCompton::NonLinearCompton(LaserField* field, UnitsSystem* units, double dt):
m_filed(field), m_units(units), m_dt(dt)
{
	LoadTables();
}

NonLinearCompton::~NonLinearCompton()
{
}

void NonLinearCompton::Interact(Particle &part, ParticleList *partList)
{
	// First we need to update the optical depth of the particle based on local values
	// Still need to decide of units and constants here
	double eta = CalculateEta(part);
	double h = Numerics::Interpolate1D(m_etaVector, m_hVector, eta);
	double deltaOD = m_dt * std::sqrt(3) * eta * h / part.GetGamma();
	part.UpdateOpticalDepth(deltaOD);

	// Now check if process hass occured. If so then emmit and react
	if (part.GetOpticalDepth() < 0.0)
	{
		// Calculate the momentum of the emmitted photns of
		ThreeVector gammaMom = part.GetMomentum() * 0.1; // Dummy method
		// Update the partles 
		part.UpdateTrack(part.GetPosition(), part.GetMomentum() - gammaMom);
		// Add new partles to the simulation 
		Particle gamma = Particle(0.0, 0.0, part.GetPosition(), gammaMom, part.GetTime(), true);
		partList->AddParticle(gamma);
		part.InitOpticalDepth();
	}
}
double NonLinearCompton::CalculateEta(const Particle &part)
{
	// The direction of the particle
	ThreeVector partDir = part.GetDirection();

	// Get the E and B fields at the particles location
	ThreeVector bField = m_filed->GetBfield(part.GetPosition(),part.GetTime());
	ThreeVector eField = m_filed->GetEfield(part.GetPosition(),part.GetTime());
	
	// Get the components of E perp and para to direction of motion
	ThreeVector ePara = eField.Dot(partDir) * partDir;
	ThreeVector ePerp = eField - ePara;

	double eleCrit = 1.0;
	double eta = std::sqrt((ePerp + part.GetBeta() * partDir.Cross(bField)).Mag2() 
						    + std::pow(partDir.Dot(eField), 2.0)) * part.GetGamma() / eleCrit;
	return eta; 
}

void NonLinearCompton::LoadTables()
{
	std::ifstream hFile("./Tables/hsokolov.table");
	if (!hFile)
	{
		std::cerr << "ERROR: Data table for h not found!" << std::endl;
		std::cerr << "Please download required data tables." << std::endl;
		exit(1); 
	}
	int nValues;
	double  logEta, logH;
	hFile >> nValues;
	m_hVector = std::vector<double>(nValues);
	m_etaVector = std::vector<double>(nValues);
	for (unsigned int i = 0; i < nValues; ++i)
	{
		hFile >> logEta >> logH;
		m_hVector[i] = std::pow(10.0, logH);
		m_etaVector[i] = std::pow(10.0, logEta);
	}
}
