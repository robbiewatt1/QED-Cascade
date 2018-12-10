#include <cmath>

#include "LaserField.hh"
#include "Constants.hh"

LaserField::LaserField()
{
}

LaserField::LaserField(double maxI, double tau, double waist, double start, double polAngle,
					   const ThreeVector &focus, const ThreeVector &waveVec):
m_maxI(maxI), m_tau(tau), m_waist(waist), m_start(start), m_waveVec(waveVec), m_focus(focus),
m_polAngle(polAngle)
{
	m_rotaion = m_waveVec.RotateToAxis(ThreeVector(0,0,1));
	m_rotationInv = m_rotaion.Inverse();
	m_lambda = 2.0 * Constants::pi / m_waveVec.Mag();
	m_rotationInv.Print();
	std::cout << waist << std::endl;
}

LaserField::~LaserField()
{
}

ThreeVector LaserField::GetEfield(const ThreeVector &position, double time) const
{
	ThreeVector position_p = m_rotaion * position;

	double r2 = position_p[0] * position_p[0] + position_p[1] * position_p[1];
	double rayleigh = Constants::pi * m_waist * m_waist / m_lambda;
	double beamWaist = m_waist * std::sqrt(1.0 + ((position_p[2] * position_p[2]) 
												 / (rayleigh * rayleigh)));
	double E0 = m_maxI * (m_waist / beamWaist) * std::exp(-1.0 * r2 / (beamWaist * beamWaist))
				* std::sin(position_p[2] / m_lambda) * std::exp(-1.0 * (time - m_start - position_p[2]) *
						(time - m_start - position_p[2]) / (m_tau * m_tau));

	ThreeVector Efield;
	Efield[0] = E0 * std::cos(m_polAngle);
	Efield[1] = E0 * std::sin(m_polAngle);
	Efield[2] = 0;
	return m_rotationInv * Efield;
}

ThreeVector LaserField::GetBfield(const ThreeVector &position, double time) const
{
}

void LaserField::SaveField(HDF5Output &file, const std::vector<double> &tAxis,
						   const std::vector<double> &xAxis, const std::vector<double> &yAxis,
						   const std::vector<double> &zAxis)
{
	file.AddGroup("Field");
	for (int t = 0; t < tAxis.size(); t++)
	{
		std::string groupName = "Field/" + std::to_string(tAxis[t]);
		groupName.erase(groupName.find_last_not_of('0') + 1, std::string::npos);
		file.AddGroup(groupName);
		for (int dir = 0; dir < 3; dir++)
		{
			double dataBuff[xAxis.size()*yAxis.size()*zAxis.size()];
			for (unsigned int i = 0; i < xAxis.size(); i++)
			{
				for (unsigned int j = 0; j < yAxis.size(); j++)
				{
					for (unsigned int k = 0; k < zAxis.size(); k++)
					{
						unsigned int index = (i*yAxis.size()*zAxis.size()) + (j*zAxis.size()) + k;
						dataBuff[index] = GetEfield(ThreeVector(xAxis[i], yAxis[j], zAxis[k]),
													tAxis[t])[dir];
					}
				}
			}
			std::string dataName = groupName + "/E" + std::to_string(dir);
			file.AddArray3D(dataBuff, xAxis.size(), xAxis.size(), xAxis.size(), dataName);
		}
	}
}