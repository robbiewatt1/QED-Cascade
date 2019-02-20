#ifndef PROCESS_HH
#define PROCESS_HH

#include "Particle.hh"
#include "ParticleList.hh"
#include "EMField.hh"

class Process
{
public:
	Process(EMField* field, double dt): m_field(field), m_dt(dt){ };
	
	virtual ~Process(){ };

	virtual void Interact(Particle *part, ParticleList *partList) const = 0;

protected:
	EMField* m_field;
	double m_dt;
};
#endif