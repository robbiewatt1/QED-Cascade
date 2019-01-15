#include <cmath>

#include "LaserField.hh"
#include "UnitsSystem.hh"

LaserField::LaserField()
{
}

LaserField::LaserField(double maxI, double tau, double waveLength, double waist, 
					   double start, double polAngle, const ThreeVector &focus,
					   const ThreeVector &waveVec):
m_maxI(maxI), m_tau(tau), m_waveLength(waveLength), m_waist(waist), m_start(start),
m_polAngle(polAngle), m_focus(focus)
{
	m_waveVec = waveVec.Norm();
	m_rotaion = m_waveVec.RotateToAxis(ThreeVector(0,0,1));
	m_rotationInv = m_rotaion.Inverse();
	m_waveNum = 2.0 * UnitsSystem::pi / m_waveLength;
}

LaserField::~LaserField()
{
}

ThreeVector LaserField::GetEfield(const ThreeVector &position, double time) const
{
	ThreeVector position_p = m_rotaion * position;
	double r2 		 = position_p[0] * position_p[0] + position_p[1] * position_p[1];
	double rayleigh  = m_waveNum * m_waist * m_waist / 2.0;
	double curvature = (position_p[2] + rayleigh * rayleigh
										/ (position_p[2] + 1e-10));
	// Adding small number to stop divergence at zero
	double beamWaist = m_waist * std::sqrt(1.0 + position_p[2] * position_p[2] 
										   / (rayleigh * rayleigh));
	double E0 = 0.5 * std::sqrt(m_maxI) 
				* (m_waist / beamWaist) 
				* std::exp(-1.0 * r2 / (beamWaist * beamWaist))
				* std::cos(position_p[2] * m_waveNum + r2 * m_waveNum / (2.0 * curvature)) 
				* std::exp(-1.0 * (time + m_start - position_p[2]) * (time + m_start - position_p[2]) / (m_tau * m_tau));
	
	ThreeVector Efield;
	Efield[0] = E0 * std::cos(m_polAngle);
	Efield[1] = E0 * std::sin(m_polAngle);
	Efield[2] = 0;
	return m_rotationInv * Efield;
}

ThreeVector LaserField::GetBfield(const ThreeVector &position, double time) const
{
	// This method is double as slow and does not need to be done. 
	// Can just calculate E and B at the same time
	ThreeVector position_p = m_rotaion * position;

	double r2 		 = position_p[0] * position_p[0] + position_p[1] * position_p[1];
	double rayleigh  = m_waveNum * m_waist * m_waist / 2.0;
	double curvature = (position_p[2] + rayleigh * rayleigh
										/ (position_p[2] + 1e-10));
	double beamWaist = m_waist * std::sqrt(1.0 + position_p[2] * position_p[2] 
										   / (rayleigh * rayleigh));
	double B0 = 0.5 * std::sqrt(m_maxI) 
				* (m_waist / beamWaist) 
				* std::exp(-1.0 * r2 / (beamWaist * beamWaist))
				* std::cos(position_p[2] * m_waveNum + r2 * m_waveNum / (2.0 * curvature)) 
				* std::exp(-1.0 * (time - m_start - position_p[2]) * (time - m_start - position_p[2]) / (m_tau * m_tau));

	ThreeVector Bfield;
	Bfield[0] = B0 * std::sin(m_polAngle);
	Bfield[1] = B0 * std::cos(m_polAngle);
	Bfield[2] = 0;
	return m_rotationInv * Bfield;
}

void LaserField::SaveField(HDF5Output &file, const std::vector<double> &tAxis,
						   const std::vector<double> &xAxis, const std::vector<double> &yAxis,
						   const std::vector<double> &zAxis)
{
	file.AddGroup("Field");
	for (unsigned int t = 0; t < tAxis.size(); t++)
	{
		std::string groupName = "Field/" + std::to_string(tAxis[t]);
		groupName.erase(groupName.find_last_not_of('0') + 1, std::string::npos);
		file.AddGroup(groupName);
		for (int dir = 0; dir < 3; dir++)
		{
			double* dataBuff = new double[xAxis.size()*yAxis.size()*zAxis.size()];
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
			file.AddArray3D(dataBuff, xAxis.size(), yAxis.size(), zAxis.size(), dataName);
			delete dataBuff;
		}
	}
}
