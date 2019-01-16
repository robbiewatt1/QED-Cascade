#include "StaticField.hh"

StaticField::StaticField(ThreeVector eField, ThreeVector bField)
{
	m_eField = eField;
	m_bField = bField;
}

StaticField::~StaticField()
{
}

void StaticField::GetField(double time, const ThreeVector &position,
						   ThreeVector &eField, ThreeVector &bField) const
{
	eField = m_eField;
	bField = m_bField;
}