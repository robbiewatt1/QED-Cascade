#ifndef LINEARBW_HH
#define LINEARBW_HH

#include "Process.hh"
#include "RadField.HH"

class LinearBW: public Process
{
public:
	LinearBW(RadField* field, double dt);

	~LinearBW();

	void Interact(Particle &part);
	
private:

	double CrossSection(double s);

	double DiffCrossSection(double s, double theta);

private:
	RadField* m_field;
	double m_dt;
};

#endif