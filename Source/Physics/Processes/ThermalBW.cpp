#include <fstream>

#include "ThermalBW.hh"
#include "Numerics.hh"
#include "UnitsSystem.hh"

ThermalBW::ThermalBW(ThermalRadField* field, double dt, double maxEnergy, std::string table,
					 bool saveTable):
m_field(field), m_dt(dt), m_maxEnergy(maxEnergy), m_saveTable(saveTable)
{
	LoadTables(table);
}

ThermalBW::~ThermalBW()
{
	delete [] m_energy;
	delete [] m_temperature;
	for (unsigned int i = 0; i < m_energySize; i++)
	{
		delete [] m_opticalDepth[i];
	}
	delete [] m_opticalDepth;
}

void ThermalBW::Interact(Particle &part)
{
	// Calculate the 
/*
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
*/
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
	if (!bwFile)
	{
		std::cout << "No table found for linear Breit-Wheeler processes" << std::endl;
		std::cout << "Table musty be generated, this may take a while!" << std::endl;
		CalculateTables();
	} else 
	{
		// load data table
	}
}

void ThermalBW::CalculateTables()
{
	m_energySize = 200;
	m_tempSize = 200;

	// Generate the energy and temperature axis of the table
	double deltaEnergy = m_maxEnergy / (m_energySize);
	m_energy = new double [m_energySize];
	for (unsigned int i = 0; i < m_energySize; i++)
	{
		m_energy[i] = (i + 1) * deltaEnergy;
	}

	double tempLimits[2];
	m_field->GetTempLimits(tempLimits);
	double deltaTemp = (tempLimits[1] - tempLimits[0]) / (m_tempSize - 1);
	m_temperature = new double [m_tempSize];
	for (unsigned int i = 0; i < m_tempSize; i++)
	{
		m_temperature[i] = tempLimits[0] + i * deltaTemp;
	}

	// Now generate the optical depth values for the table
	m_opticalDepth = new double* [m_energySize];

	// Arrays that are used in integration
	double* epsilon = new double [m_intSize];
	double* epsilonInt = new double [m_intSize];
	double* s = new double [m_intSize];
	double* sInt = new double [m_intSize];
	for (unsigned int i = 0; i < m_energySize; i++)
	{
		// For progress bar
		unsigned int barLength = 50;
		double progress = (double)i / m_energySize;
		unsigned int pos = progress * barLength;
		std::cout << "[";
		for(unsigned int j = 0; j != barLength; j++)
		{
   			 if(j < pos) {std::cout << "|";}	
    		else {std::cout << " ";}
		}
		std::cout << "]\r";

		m_opticalDepth[i] = new double [m_tempSize];
		for (unsigned int j = 0; j < m_tempSize; j++)
		{
			double nu = 1.0 / (m_energy[i] * m_temperature[j]);
			// need to first calculate f, required two integrals. Inner one over s
			// and the outter one of com energy xray energy

			// integral to inf. Needto check what value of epsMax this should be
			// Use 20* temp for now
			double deltaEpsilon = 10 * m_temperature[j] / m_intSize;

			for (unsigned int k = 0; k < m_intSize; k++)
			{
				epsilon[k] = nu + (k + 1) * deltaEpsilon;

				double deltaS = (epsilon[k] / nu - 1.0) / (m_intSize); 
				for (unsigned int l = 0; l < m_intSize; l++)
				{
					s[l] = 1.0 + l * deltaS;
					sInt[l] = s[l] * CrossSection(s[l]);
				}
				epsilonInt[k] = Numerics::SimpsonsRule(s, sInt, m_intSize) 
								/ (std::exp(epsilon[k]) - 1.0);
				std::cout << Numerics::SimpsonsRule(s, sInt, m_intSize) << std::endl;

			}
			getchar();
			m_opticalDepth[i][j] = Numerics::SimpsonsRule(epsilon, epsilonInt, m_intSize)
								  * m_temperature[j] * m_temperature[j] * m_temperature[j]
								  * 2.0 * nu * nu / UnitsSystem::pi;
		}
	}

	// Clean up
	delete [] epsilon;
	delete [] epsilonInt;
	delete [] s;
	delete [] sInt;

	if (m_saveTable == true)
	{
		std::ofstream odTable("./BWtable.table");
		odTable << m_energy[0] << "\t" << m_energy[m_energySize-1] << "\t" << m_energySize << "\n"
				<< m_temperature[0] << "\t" << m_temperature[m_energySize-1] << "\t" <<  m_tempSize << "\n";
		for (unsigned int i = 0; i < m_energySize; i++)
		{
			for (unsigned int j = 0; j < m_tempSize; j++)
			{
				odTable << m_opticalDepth[i][j] << "\t";
			}
			odTable << "\n";
		}
	}
}
