#ifndef STATICFIELD_HH
#define STATICFIELD_HH

#include "Field.hh"
#include "ThreeVector.hh"

class StaticField: public Field
{
public:
	StaticField(ThreeVector eField, ThreeVector bField);

	virtual ~StaticField();
	
	virtual void GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const;

private:
	ThreeVector m_eField;
	ThreeVector m_bField;
};
#endif