#include "StaticEMField.hh"

StaticEMField::StaticEMField(ThreeVector eField, ThreeVector bField)
{
	m_eField = eField;
	m_bField = bField;
}

StaticEMField::~StaticEMField()
{
}

void StaticEMField::GetField(double time, const ThreeVector &position,
						     ThreeVector &eField, ThreeVector &bField) const
{
	eField = m_eField;
	bField = m_bField;
}