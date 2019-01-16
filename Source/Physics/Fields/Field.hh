#ifndef FIELD_HH
#define FIELD_HH

#include "ThreeVector.hh"

class Field
{
public:
	Field(){};

	virtual ~Field(){};

	virtual void GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const = 0;
};

#endif