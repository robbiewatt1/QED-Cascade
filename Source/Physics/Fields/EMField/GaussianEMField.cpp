#include <cmath>

#include "GaussianEMField.hh"
#include "UnitsSystem.hh"

GaussianEMField::GaussianEMField()
{
}

GaussianEMField::GaussianEMField(double maxE,  double waveLength, double tau, double waist,
					   double polAngle, const ThreeVector& startPos,
					   const ThreeVector& focusPos):
m_maxE(maxE), m_waveLength(waveLength), m_tau(tau), m_waist(waist), m_polAngle(polAngle)
{
	m_startPos = startPos;
	m_focusPos = focusPos;
	m_waveVec = startPos.Direction(focusPos);
	m_rotaion = m_waveVec.RotateToAxis(ThreeVector(0,0,1));
	m_rotationInv = m_rotaion.Inverse();
	m_waveNum = 2.0 * UnitsSystem::pi / m_waveLength;
	std::cout << maxE << " " <<m_waveLength << std::endl;
}

GaussianEMField::~GaussianEMField()
{
}

void GaussianEMField::GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const
{
	ThreeVector position_p = m_rotaion * position;
	double t0 		 = (m_startPos - m_focusPos).Mag();
	double r2 		 = position_p[0] * position_p[0] + position_p[1] * position_p[1];
	double rayleigh  = m_waveNum * m_waist * m_waist / 2.0;
	double curvature = (position_p[2] + rayleigh * rayleigh
										/ (position_p[2] + 1e-99));
	// Adding small number to stop divergence at zero
	double beamWaist = m_waist * std::sqrt(1.0 + position_p[2] * position_p[2] 
										   / (rayleigh * rayleigh));
	double E0 = m_maxE * (m_waist / beamWaist) 
				* std::exp(-1.0 * r2 / (beamWaist * beamWaist))
				* std::cos((position_p[2] - time) * m_waveNum + r2 * m_waveNum / (2.0 * curvature))
				* std::exp(-1.0 * (time - t0 - position_p[2]) 
							* (time - t0 - position_p[2]) / (m_tau * m_tau));
	

	eField[0] = E0 * std::cos(m_polAngle);
	eField[1] = E0 * std::sin(m_polAngle);
	eField[2] = 0;
	eField = m_rotationInv * eField;

	bField[0] = E0 * std::sin(m_polAngle);
	bField[1] = E0 * std::cos(m_polAngle);
	bField[2] = 0;
	bField = m_rotationInv * bField;
}


/*
void GaussianEMField::SaveField(HDF5Output &file, const std::vector<double> &tAxis,
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
*/