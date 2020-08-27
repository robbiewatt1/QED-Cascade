#include <cmath>
#include <fstream>

#include "NonLinearCompton.hh"
#include "Photon.hh"
#include "Numerics.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"


NonLinearCompton::NonLinearCompton(EMField* field, double dt, bool track,
	double eMin):
m_eMin(eMin), Process(field, dt, track)
{
	LoadTables();
}

NonLinearCompton::~NonLinearCompton()
{
	UnloadTables();
}

void NonLinearCompton::Interact(Particle *part, ParticleList *partList) const
{
	if (part->GetMass() == 0 || part->IsAlive() == false) return;
	// First we need to update the optical depth of the particle based on local values
	// Still need to decide of units and constants here
	double eta = CalculateEta(part);

	// Check for very small values of eta and skip interpolation
	double logh;
	if (eta < 1.0e-12)
	{
		logh = 5.24;
	} else
	{
		logh = Numerics::Interpolate1D(m_h_etaAxis, m_h_dataTable,
			m_h_length, std::log10(eta));
	}
	double deltaOD = m_dt * std::sqrt(3) * UnitsSystem::alpha * eta
		* std::pow(10.0, logh)
		/ (part->GetGamma() * 2.0 * UnitsSystem::pi);
	part->UpdateOpticalDepth(deltaOD);
	// Now check if process hass occured. If so then emmit and react
	if (part->GetOpticalDepth() < 0.0)
	{
		double chi = CalculateChi(eta);
		double gammaE = 2.0 * chi * part->GetGamma() / eta;
		ThreeVector gammaP = gammaE * part->GetDirection();
		part->UpdateTrack(part->GetPosition(), part->GetMomentum() - gammaP);
		// Add new partles to the simulation
		if (gammaE > m_eMin)
		{
			Photon* photon = new Photon(gammaE, part->GetPosition(), part->GetDirection(), 
								  	   part->GetTime(), m_track);
			partList->AddParticle(photon);
		}
		part->InitOpticalDepth();
	}
}

double NonLinearCompton::CalculateEta(Particle* part) const
{
	ThreeVector partDir = part->GetDirection();
	ThreeVector eField, bField;
	m_field->GetField(part->GetTime(), part->GetPosition(), eField, bField);
	ThreeVector ePara = eField.Dot(partDir) * partDir;
	ThreeVector ePerp = eField - ePara;
	double eta = std::sqrt((ePerp + part->GetBeta() * partDir.Cross(bField)).Mag2()
						   + std::pow(partDir.Dot(eField), 2.0) / (part->GetGamma() 
						   	* part->GetGamma())) * part->GetGamma();
	return eta;
}

double NonLinearCompton::CalculateChi(double eta) const
{
	double rand = MCTools::RandDouble(0, 1);
	int lowIndex;
	double frac;
	Numerics::ClosestPoints(m_phEn_etaAxis, m_phEn_etaLength, std::log10(eta),
		lowIndex, frac);
	double lowValue = Numerics::Interpolate1D(m_phEn_dataTable[lowIndex],
		m_phEn_chiAxis[lowIndex], m_phEn_chiLength, rand);
	double highValue = Numerics::Interpolate1D(m_phEn_dataTable[lowIndex+1],
		m_phEn_chiAxis[lowIndex+1], m_phEn_chiLength, rand);
	return std::pow(10.0, (1.0 - frac) * lowValue + frac * highValue);
}

void NonLinearCompton::LoadTables()
{
	char* tablePath(getenv("QED_TABLES_PATH"));
	if (tablePath == NULL)
	{
		std::cout << "Error: Enviromental variable \"QED_TABLES_PATH\" "; 
		std::cout << "is not set!" << std::endl;
		std::cout <<  "Please set QED_TABLES_PATH to point to tables directory." << std::endl;
	}

	std::string path(tablePath);
	// Load the table for h
	std::ifstream hFile(path + "/hsokolov.table");
	if (!hFile)
	{
		std::cerr << "ERROR: Data table for h not found!" << std::endl;
		std::cerr << "No file at: " << path + "/hsokolov.table" << std::endl;
		exit(1);
	}

	std::ifstream phEnFile(path + "/ksi_sokolov.table");
	if (!phEnFile)
	{
		std::cerr << "ERROR: Data table for photon energy sampling not found!" << std::endl;
		std::cerr << "No file at: " << path + "/ksi_sokolov.table" << std::endl;
		exit(1); 
	}
	std::ifstream chiMinFile(path + "/chimin.table");
	if (!chiMinFile)
	{
		std::cerr << "ERROR: Data table for chimin energy sampling not found!" << std::endl;
		std::cerr << "No file at: " << path + "/chimin.table" << std::endl;
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
		m_h_etaAxis[i] = logEta;
		m_h_dataTable[i] = logH;

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
		double logMinChi = std::log10(m_phEn_chiMinAxis[i]);
		m_phEn_etaAxis[i] = logMinEta + i * deltaEta;
		double deltaChi = (m_phEn_etaAxis[i] - std::log10(2.0)
			- logMinChi)  / (m_phEn_etaLength - 1.0);
		for (unsigned int j = 0; j < m_phEn_chiLength; j++)
		{
			m_phEn_chiAxis[i][j] = logMinChi + j * deltaChi;
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