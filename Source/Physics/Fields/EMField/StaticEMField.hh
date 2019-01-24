#ifndef StaticEMField_HH
#define StaticEMField_HH

#include "EMField.hh"
#include "ThreeVector.hh"

class StaticEMField: public EMField
{
public:
	StaticEMField(ThreeVector eField, ThreeVector bField);

	virtual ~StaticEMField();
	
	virtual void GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const;

private:
	ThreeVector m_eField;
	ThreeVector m_bField;
};
#endif