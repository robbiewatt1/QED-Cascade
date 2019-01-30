#include <cmath>

#include "ThermalRadField.hh"
#include "UnitsSystem.hh"

ThermalRadField::ThermalRadField(double temp, double maxEnergy):
m_temp(temp), m_maxEnergy(maxEnergy)
{
}

ThermalRadField::~ThermalRadField()
{
}

void ThermalRadField::GetField(double time, const ThreeVector &position, double* energy,
							   double* density, unsigned int resolusion) const
{
	double deltaE = m_maxEnergy / resolusion;
	for (unsigned int i = 0; i < resolusion; i++)
	{
		energy[i] = i * resolusion;
		density[i] = 1.0 / (UnitsSystem::pi * UnitsSystem::pi) * energy[i] * energy[i]
					 / (std::exp(energy[i] / m_temp) - 1.0); 
	}
}

void ThermalRadField::GetTempLimits(double limits[2])
{
	limits[0] = m_temp;
	limits[1] = 10 * m_temp;
}