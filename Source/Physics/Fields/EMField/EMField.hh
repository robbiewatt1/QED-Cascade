#ifndef EMFIELD_HH
#define EMFIELD_HH

#include "ThreeVector.hh"

class EMField
{
public:
	EMField(){};

	virtual ~EMField(){};

	virtual void GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const = 0;
};

#endif