#ifndef LANDAULIFSHITZ_HH
#define LANDAULIFSHITZ_HH

class LandauLifshitz: public Process 
{
public:
	LandauLifshitz(EMField* field, double dt);

	virtual ~LandauLifshitz();
};
#endif