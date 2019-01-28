#include "LinearBW.hh"

LinearBW::LinearBW(RadField* field, double dt, std::string table, bool saveTable):
m_field(field), m_dt(dt), m_saveTable(saveTable)
{
	LoadTables(table)
}

LinearBW::~LinearBW()
{
}

void LinearBW::Interact(Particle &part)
{
	// Calculate the 

	// Check if the interaction happened
	if (part.GetOpticalDepth() < 0.0)
	{
		part.InitOpticalDepth();
		if (m_genSecondary == true)
		{
			Particle electron = Particle(0.0, 0.0, part.GetPosition(), gammaP, part.GetTime(), false);
			Particle positron = Particle(0.0, 0.0, part.GetPosition(), gammaP, part.GetTime(), false);
			
			m_secondaries->AddParticle(gamma);
			m_secondaries->AddParticle(positron);
		}
	}
}

double LinearBW::CrossSection(double s)
{
	// should just have this fuunction in terms of s!
	double beta = std::sqrt(1.0 - 1.0 / s);
	return 0.5 * UnitsSystem::pi * UnitsSystem::alpha * UnitsSystem::alpha
		   * (1.0 - beta * beta) * (-2.0 * beta * (2.0 - beta * beta) 
		   + (3.0 - beta * beta * beta * beta) * std::log((1.0 + beta) / (1.0 - beta)));
}

double LinearBW::DiffCrossSection(double s, double theta)
{

	double beta = std::sqrt(1.0 - 1.0 / s);
	double cost = std::cos(theta);
	return 	UnitsSystem::alpha * UnitsSystem::alpha * beta / s 
			* (-1.0 + ((3.0 - beta * beta * beta * beta) / 2.0)
			* (1.0 / (1.0 - beta * cost) + 1.0 / (1.0 + beta * cost))
			- (2.0 / (s * s)) * (1.0 / ((1.0 - beta * cost) * (1.0 - beta * cost))
							   + 1.0 / ((1.0 + beta * cost) * (1.0 + beta * cost))));
}

void LinearBW::LoadTables(std::string table)
{
	std::ifstream bwFile(table);
	if (!hFile)
	{
		std::cout << "No table found for linear Breit-Wheeler processes" << std::endl;
		std::cout << "Table musty be generated, this may take a while!" << std::endl;
	} else 
	{
		// load data table
	}
}

void LinearBW::GenerateTables()
{
	// first generate optical depth table
	m_od_length = 500;
	double energyMax = 2e3; // max energy ~ 1GeV

	double deltaEnergy = energyMax / (m_od_length - 1);
	m_od_energyAxis = new double [m_od_length];
	for (unsigned int i = 0; i < m_od_length; i++)
	{
		m_od_energyAxis[i] = i * deltaEnergy;
	}

	for (int i = 0; i < count; ++i)
	{
		// solve inner integral
		double sAxis = 
	}
}