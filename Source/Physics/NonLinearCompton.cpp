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
	/*
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
	*/
}
double NonLinearCompton::CalculateEta(const Particle &part)
{
	/*
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
	*/
	return 0;
}

void NonLinearCompton::LoadTables()
{
	// Load the table for h
	std::ifstream hFile("../../Tables/hsokolov.table");
	if (!hFile)
	{
		std::cerr << "ERROR: Data table for h not found!" << std::endl;
		std::cerr << "Please download required data tables." << std::endl;
		exit(1);
	}

	hFile >> m_h_length;
	m_h_dataTable = new double [m_h_length];
	m_h_etaAxis   = new double [m_h_length];
	double  logEta, logH;
	for (unsigned int i = 0; i < m_h_length; i++)
	{
		hFile >> logEta >> logH;
		m_h_dataTable[i] = std::pow(10.0, logEta);
		m_h_etaAxis[i] = std::pow(10.0, logH);
	}

	// Load the table photon energy
	std::ifstream phEnFile("../../Tables/ksi_sokolov.table");
	if (!phEnFile)
	{
		std::cerr << "ERROR: Data table for photon energy sampling not found!" << std::endl;
		std::cerr << "Please download required data tables." << std::endl;
		exit(1); 
	}

	double logMaxEta, logMinEta;
	phEnFile >> m_phEn_etaLength >> m_phEn_chiLength >> logMinEta >> logMaxEta;
	m_phEn_etaAxis = new double [m_phEn_etaLength];
	m_phEn_chiAxis = new double* [m_phEn_etaLength];
	m_phEn_dataTable = new double* [m_phEn_etaLength];
	for (unsigned int i = 0; i < m_phEn_etaLength; i++)
	{
		m_phEn_chiAxis[i] = new double [m_phEn_chiLength];
		m_phEn_dataTable[i] = new double [m_phEn_chiLength];
	}

	for (unsigned int i = 0; i < m_phEn_etaLength; i++)
	{
		for (unsigned int j = 0; j < m_phEn_chiLength; j++)
		{
			phEnFile >> m_phEn_dataTable[i][j];
			std::cout << m_phEn_dataTable[i][j] << "\t";
		}
		std::cout << "\n";
	}
}

void NonLinearCompton::UnloadTables()
{
	delete [] m_h_dataTable;
	delete [] m_h_etaAxis;

	for (unsigned int i = 0; i < m_phEn_etaLength; i++)
	{
		delete [] m_phEn_chiAxis[i];
		delete [] m_phEn_dataTable[i];
	}
	delete [] m_phEn_chiAxis;
	delete [] m_phEn_dataTable;
	delete [] m_phEn_etaAxis;
}