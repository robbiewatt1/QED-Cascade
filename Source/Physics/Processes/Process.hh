#ifndef PROCESS_HH
#define PROCESS_HH

#include "Particle.hh"
#include "ParticleList.hh"

class Process
{
public:
	Process();
	
	virtual ~Process();

	virtual void Interact(Particle &part) = 0;

	void SetSecondaryList(ParticleList* partList);

protected:
	bool m_genSecondary;
	ParticleList* m_secondaries;
};
#endif