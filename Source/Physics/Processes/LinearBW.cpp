#include "ThermalBW.hh"
#include "Numerics.hh
"
ThermalBW::ThermalBW(RadField* field, double dt, double maxEnergy, std::string table,
					 bool saveTable):
m_field(field), m_dt(dt), m_maxEnergy(maxEnergy), m_saveTable(saveTable)
{
	LoadTables(table)
}

ThermalBW::~ThermalBW()
{
}

void ThermalBW::Interact(Particle &part)
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

double ThermalBW::CrossSection(double s)
{
	// should just have this fuunction in terms of s!
	double beta = std::sqrt(1.0 - 1.0 / s);
	return 0.5 * UnitsSystem::pi * UnitsSystem::alpha * UnitsSystem::alpha
		   * (1.0 - beta * beta) * (-2.0 * beta * (2.0 - beta * beta) 
		   + (3.0 - beta * beta * beta * beta) * std::log((1.0 + beta) / (1.0 - beta)));
}

double ThermalBW::DiffCrossSection(double s, double theta)
{

	double beta = std::sqrt(1.0 - 1.0 / s);
	double cost = std::cos(theta);
	return 	UnitsSystem::alpha * UnitsSystem::alpha * beta / s 
			* (-1.0 + ((3.0 - beta * beta * beta * beta) / 2.0)
			* (1.0 / (1.0 - beta * cost) + 1.0 / (1.0 + beta * cost))
			- (2.0 / (s * s)) * (1.0 / ((1.0 - beta * cost) * (1.0 - beta * cost))
							   + 1.0 / ((1.0 + beta * cost) * (1.0 + beta * cost))));
}

void ThermalBW::LoadTables(std::string table)
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

void ThermalBW::GenerateTables()
{

	m_energySize = 200;
	m_tempSize = 200;

	double deltaEnergy = m_maxEnergy / (m_energySize - 1);
	m_energyAxis = new double [m_energySize];
	for (unsigned int i = 0; i < m_energySize; i++)
	{
		m_energyAxis[i] = i * deltaEnergy;
	}

	double tempLimits[2];
	field->GetTempLimits(tempLimits);
	double deltaTemp = (tempLimits[1] - tempLimits[0]) / (m_tempSize - 1);
	m_tempAxis = new double [m_tempSize];
	for (unsigned int i = 0; i < m_tempSize; i++)
	{
		m_tempAxis[i] = tempLimits[0] + i * deltaTemp;
	}

	m_ODdataTable = new double* [m_energySize];
	for (unsigned int i = 0; i < m_energySize; i++)
	{
		m_ODdataTable[i] = new double [m_tempSize];
		for (int j = 0; j < m_tempSize; j++)
		{
			double nu = 1.0 / (m_energyAxis[i] * m_tempAxis[j]);

			// need to first calculate f, required two integrals. Inner one over s
			// and the outter one of com energy xray energy

			// generate the epsilon integrand
			for (int i = 0; i < count; ++i)
			{
				/* code */
			}

			// calculate s integral



			


			m_ODdataTable[i][j] = 0;
		}
	}
}