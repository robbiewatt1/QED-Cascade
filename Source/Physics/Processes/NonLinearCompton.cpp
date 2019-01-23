#include <cmath>
#include <fstream>

#include "NonLinearCompton.hh"
#include "Numerics.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"


NonLinearCompton::NonLinearCompton(Field* field, UnitsSystem* units, double dt):
m_filed(field), m_units(units), m_dt(dt)
{
	MCTools::SetSeed(5);
	LoadTables();
}

NonLinearCompton::~NonLinearCompton()
{
	UnloadTables();
}

void NonLinearCompton::Interact(Particle &part, ParticleList *partList)
{
	// First we need to update the optical depth of the particle based on local values
	// Still need to decide of units and constants here
	double eta = CalculateEta(part);
	double h = Numerics::Interpolate1D(m_h_etaAxis, m_h_dataTable, m_h_length, eta);
	double deltaOD = m_dt * std::sqrt(3) * UnitsSystem::alpha * eta * h / part.GetGamma();
	part.UpdateOpticalDepth(deltaOD);

	// Now check if process hass occured. If so then emmit and react
	if (part.GetOpticalDepth() < 0.0)
	{
		double chi = CalculateChi(eta);
		double gammaE = chi * part.GetGamma() / eta;
		ThreeVector gammaP = gammaE * part.GetDirection();
		
		part.UpdateTrack(part.GetPosition(), part.GetMomentum() - gammaP);
		// Add new partles to the simulation 
		Particle gamma = Particle(0.0, 0.0, part.GetPosition(), gammaP, part.GetTime(), false);
		partList->AddParticle(gamma);
		part.InitOpticalDepth();
	}
}

double NonLinearCompton::CalculateEta(const Particle &part)
{
	ThreeVector partDir = part.GetDirection();
	ThreeVector eField, bField;
	m_filed->GetField(part.GetTime(), part.GetPosition(), eField, bField);
	// Get the components of E perp and para to direction of motion
	ThreeVector ePara = eField.Dot(partDir) * partDir;
	ThreeVector ePerp = eField - ePara;
	double eta = std::sqrt((ePerp + part.GetBeta() * partDir.Cross(bField)).Mag2()
						   + std::pow(partDir.Dot(eField), 2.0) / (part.GetGamma() 
						   	* part.GetGamma())) * part.GetGamma();
	return eta;
}

double NonLinearCompton::CalculateChi(double eta)
{
	double rand = MCTools::RandDouble(0,1);

	// Need to first find closest index for given eta;
	int etaIndex = Numerics::ArrayIndex(m_phEn_etaAxis, m_phEn_etaLength, eta);
	// Now need to find closest index for random value
	int chiIndex = Numerics::ArrayIndex(m_phEn_dataTable[etaIndex], m_phEn_etaLength, rand);
	// Now find the chi value from the table
	return m_phEn_chiAxis[etaIndex][chiIndex];
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

	std::ifstream phEnFile("../../Tables/ksi_sokolov.table");
	if (!phEnFile)
	{
		std::cerr << "ERROR: Data table for photon energy sampling not found!" << std::endl;
		std::cerr << "Please download required data tables." << std::endl;
		exit(1); 
	}
	std::ifstream chiMinFile("../../Tables/chimin.table");
	if (!chiMinFile)
	{
		std::cerr << "ERROR: Data table for chimin energy sampling not found!" << std::endl;
		std::cerr << "Please download required data tables." << std::endl;
		exit(1); 
	}

	// h table
	hFile >> m_h_length;
	m_h_dataTable = new double [m_h_length];
	m_h_etaAxis   = new double [m_h_length];
	double  logEta, logH;
	for (unsigned int i = 0; i < m_h_length; i++)
	{
		hFile >> logEta >> logH;
		m_h_etaAxis[i] = std::pow(10.0, logEta);
		m_h_dataTable[i] = std::pow(10.0, logH);

	}

	// photon energy and chi min tables
	double logMaxEta, logMinEta;
	phEnFile >> m_phEn_etaLength >> m_phEn_chiLength >> logMinEta >> logMaxEta;
	m_phEn_etaAxis = new double [m_phEn_etaLength];
	m_phEn_chiMinAxis = new double [m_phEn_etaLength];
	m_phEn_chiAxis = new double* [m_phEn_etaLength];
	m_phEn_dataTable = new double* [m_phEn_etaLength];
	for (unsigned int i = 0; i < m_phEn_etaLength; i++)
	{
		m_phEn_chiAxis[i] = new double [m_phEn_chiLength];
		m_phEn_dataTable[i] = new double [m_phEn_chiLength];
	}

	// Load or calculate each table
	for (unsigned int i = 0; i < m_phEn_etaLength; i++)
	{
		chiMinFile >> m_phEn_chiMinAxis[i];
	}

	double deltaEta = (logMaxEta - logMinEta) / (m_phEn_etaLength - 1.0);
	for (unsigned int i = 0; i < m_phEn_etaLength; i++)
	{
		m_phEn_etaAxis[i] = std::pow(10.0, logMinEta + i * deltaEta);
		double minChi = std::log10(m_phEn_chiMinAxis[i]);
		double deltaChi = (std::log10(m_phEn_etaAxis[i]) - std::log10(2.0) - minChi) 
						  / (m_phEn_etaLength - 1.0);
		for (unsigned int j = 0; j < m_phEn_chiLength; j++)
		{
			m_phEn_chiAxis[i][j] = std::pow(10.0, minChi + j * deltaChi);
		}
	}

	for (unsigned int i = 0; i < m_phEn_etaLength; i++)
	{
		for (unsigned int j = 0; j < m_phEn_chiLength; j++)
		{
			phEnFile >> m_phEn_dataTable[i][j];
		}
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
	delete [] m_phEn_chiMinAxis;
	delete [] m_phEn_etaAxis;
	delete [] m_phEn_chiAxis;
	delete [] m_phEn_dataTable;
}