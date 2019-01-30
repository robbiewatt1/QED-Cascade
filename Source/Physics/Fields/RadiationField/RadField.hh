#ifndef RADFIELD_HH
#define RADFIELD_HH

#include "ThreeVector.hh"

class RadField
{
public:
	RadField(){};

	virtual ~RadField(){};

	virtual void GetField(double time, const ThreeVector &position, double* energy,
				 		  double* density, unsigned int resolusion) const = 0;
};
#endif