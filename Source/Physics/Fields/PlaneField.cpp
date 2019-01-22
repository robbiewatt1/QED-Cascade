#include "PlaneField.hh"
#include "ThreeVector.hh"
#include "UnitsSystem.hh"


PlaneField::PlaneField()
{
}

PlaneField::PlaneField(double maxE, double waveLength, double polAngle,
					   const ThreeVector& direction):
m_maxE(maxE), m_waveLength(waveLength), m_polAngle(polAngle)
{
	m_rotaion = direction.RotateToAxis(ThreeVector(0,0,1));
	m_rotationInv = m_rotaion.Inverse();
	m_waveNum = 2.0 * UnitsSystem::pi / m_waveLength;
}

PlaneField::~PlaneField()
{
}

void PlaneField::GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const
{
	ThreeVector position_p = m_rotaion * position;
	double E0 = m_maxE * std::cos((position_p[2] - time) * m_waveNum);
	eField[0] = E0 * std::cos(m_polAngle);
	eField[1] = E0 * std::sin(m_polAngle);
	eField[2] = 0;
	eField = m_rotationInv * eField;

	bField[0] = E0 * std::sin(m_polAngle);
	bField[1] = E0 * std::cos(m_polAngle);
	bField[2] = 0;
	bField = m_rotationInv * bField;
}