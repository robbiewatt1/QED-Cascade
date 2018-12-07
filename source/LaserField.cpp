#include <cmath>

#include "LaserField.hh"
#include "Constants.hh"

LaserField::LaserField()
{
}

LaserField::LaserField(double maxI, double tau, double lambda, double waist, 
					   const ThreeVector &waveNum):
m_maxI(maxI), m_tau(tau), m_lambda(lambda), m_waveNum(waveNum), m_waist(waist)
{
}

LaserField::~LaserField()
{
}

ThreeVector LaserField::GetEfield(const ThreeVector &position, double time) const
{
	// At the moment just assume in z direction and x polerisation
	ThreeVector Efield;
	double r2 = position[0] * position[0] + position[1] * position[1];
	Efield[0] = m_maxI * m_waist / BeamWaist(position[2])
				* std::exp(-1.0 * r2 /(BeamWaist(position[2]) * BeamWaist(position[2])))
				* std::sin(position[2] / m_lambda) * std::exp();
	Efield[1] = 0;
	Efield[2] = 0;

	return Efield;
}

ThreeVector LaserField::GetBfield(const ThreeVector &position, double time) const
{
}

double LaserField::BeamWaist(double z) const
{
	double rayleigh = Constants::pi * m_waist * m_waist / m_lambda;
	return m_waist * std::sqrt(1.0 + ((z * z) / (rayleigh * rayleigh)));
}

void LaserField::SaveField(HDF5Output &file, double initT, double endT, const std::vector<double> &xAxis,
				   const std::vector<double> &yAxis, const std::vector<double> &zAxis)
{
	double dataBuff[xAxis.size()*yAxis.size()*zAxis.size()];
	for (unsigned int i = 0; i < xAxis.size(); i++)
	{
		for (unsigned int j = 0; j < yAxis.size(); j++)
		{
			for (unsigned int k = 0; k < zAxis.size(); k++)
			{
				unsigned int index = (i*yAxis.size()*zAxis.size()) + (j*zAxis.size()) + k;
				dataBuff[index] = GetEfield(ThreeVector(xAxis[i], yAxis[j], zAxis[k]), 0)[0];
			}
		}
	}
	file.AddArray3D(dataBuff, xAxis.size(), xAxis.size(), xAxis.size(), "Efield");
}