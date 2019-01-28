#ifndef THERMALRADFIELD_HH
#define THERMALRADFIELD_HH

#include "RadField.hh"

class ThermalRadField: public RadField
{
public:
	ThermalRadField(double temp, double maxEnergy = 100);
	
	~ThermalRadField();

	void GetField(double time, const ThreeVector &position, double* energy,
				  double* density, unsigned int resolusion) const;

private:

	double m_temp;
	double m_maxEnergy;
	double* m_eAxis;
};
#endif