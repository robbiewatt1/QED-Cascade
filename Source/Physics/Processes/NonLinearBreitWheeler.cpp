#include <fstream>

#include "NonLinearBreitWheeler.hh"
#include "Lepton.hh"
#include "Numerics.hh"
#include "UnitsSystem.hh"
#include "MCTools.hh"

NonLinearBreitWheeler::NonLinearBreitWheeler(EMField* field, double dt):
Process(field, dt)
{
	LoadTables();
}

NonLinearBreitWheeler::~NonLinearBreitWheeler()
{
	UnloadTables();
}

void NonLinearBreitWheeler::Interact(Particle *part, ParticleList *partList) const
{
	if (part->GetType() != "Photon") return;

	double chi = CalculateChi(part);
	double t = Numerics::Interpolate1D(m_t_chiAxis, m_t_dataTable, m_t_length, chi);
	double deltaOD = m_dt * UnitsSystem::alpha * chi * t / part->GetEnergy();
	part->UpdateOpticalDepth(deltaOD);

	// Now check if process hass occured. If so then emmit and react
	if (part->GetOpticalDepth() < 0.0)
	{
	
		double split = CalculateSplit(chi);
		double pEnergy = split * part->GetEnergy();
		double eEnergy = (1.0 - split) * part->GetEnergy();
		ThreeVector pMomentum =  std::sqrt(pEnergy * pEnergy - 1.0) * part->GetDirection();
		ThreeVector eMomentum =  std::sqrt(eEnergy * eEnergy - 1.0) * part->GetDirection();
		Lepton* positron = new Lepton(1.0, 1.0, part->GetPosition(), 
									  pMomentum, part->GetTime(), false);	
		Lepton* electron = new Lepton(1.0, -1.0, part->GetPosition(),
									  eMomentum, part->GetTime(), false);
	
		partList->AddParticle(positron);
		partList->AddParticle(electron);
		part->Kill();
	}
}

double NonLinearBreitWheeler::CalculateSplit(double chi) const
{
	double rand = MCTools::RandDouble(0,1);
	int chiIndex = Numerics::ArrayIndex(m_eFract_chiAxis, m_efract_length, chi);
	int fracIndex = Numerics::ArrayIndex(m_eFract_dataTable[chiIndex], m_efract_length, rand);
	return m_eFract_fractAxis[fracIndex];
}

double NonLinearBreitWheeler::CalculateChi(Particle* part) const
{
	ThreeVector partDir = part->GetDirection();
	ThreeVector eField, bField;
	m_field->GetField(part->GetTime(), part->GetPosition(), eField, bField);
	ThreeVector ePara = eField.Dot(partDir) * partDir;
	ThreeVector ePerp = eField - ePara;
	return part->GetEnergy() * (ePerp + partDir.Cross(bField)).Mag();
}

void NonLinearBreitWheeler::LoadTables()
{
	char* tablePath(getenv("QED_TABLES_PATH"));
	if (tablePath == NULL)
	{
		std::cout << "Error: Enviromental variable \"QED_TABLES_PATH\" "; 
		std::cout << "is not set!" << std::endl;
		std::cout <<  "Please set QED_TABLES_PATH to point to tables directory." << std::endl;
	}

	std::string path(tablePath);
	std::ifstream tFile(path + "/pairprod.table");
	if (!tFile)
	{
		std::cerr << "ERROR: Data table for T not found!" << std::endl;
		std::cerr << "No file at: " << path + "/pairprod.table" << std::endl;
		exit(1);
	}

	std::ifstream fractFile(path + "/e_split.table");
	if (!fractFile)
	{
		std::cerr << "ERROR: Data table for pair energy split not found!" << std::endl;
		std::cerr << "No file at: " << path + "/e_split.table" << std::endl;
		exit(1);
	}

	std::ifstream epsilonFile(path + "/epsilon.table");
	if (!epsilonFile)
	{
		std::cerr << "ERROR: Data table for epsilon split not found!" << std::endl;
		std::cerr << "No file at: " << path + "/ epsilon.table" << std::endl;
		exit(1);
	}

	std::ifstream chiFile(path + "/log_chi.table");
	if (!chiFile)
	{
		std::cerr << "ERROR: Data table for log chi not found!" << std::endl;
		std::cerr << "No file at: " << path + "/ log_chi.table" << std::endl;
		exit(1);
	}

	tFile >> m_t_length;
	m_t_chiAxis = new double [m_t_length];
	m_t_dataTable = new double [m_t_length];
	
	double logChi, logTable;
	for (unsigned int i = 0; i < m_t_length; i++)
	{
		tFile >> logChi >> logTable;
		m_t_chiAxis[i]   = std::pow(10, logChi);
		m_t_dataTable[i] = std::pow(10, logTable);
	}

	fractFile >> m_efract_length;
	m_eFract_chiAxis = new double [m_efract_length];
	m_eFract_fractAxis = new double [m_efract_length];
	m_eFract_dataTable = new double* [m_efract_length];
	
	for (unsigned int i = 0; i < m_efract_length; i++)
	{
		chiFile >> logChi;
		m_eFract_chiAxis[i] = std::pow(10, logChi);
		epsilonFile >> m_eFract_fractAxis[i];
		m_eFract_dataTable[i] = new double [m_efract_length];
		for (unsigned int j = 0; j < m_efract_length; j++)
		{
			fractFile >> m_eFract_dataTable[i][j];
		}
	}
}

void NonLinearBreitWheeler::UnloadTables()
{
	delete [] m_t_dataTable;
	delete [] m_t_chiAxis;
	delete [] m_eFract_chiAxis;
	delete [] m_eFract_fractAxis;
	for (unsigned int i = 0; i < m_efract_length; i++)
	{
		delete [] m_eFract_dataTable[i];
	}
	delete [] m_eFract_dataTable;
}