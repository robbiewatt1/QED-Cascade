#ifndef NONLINEARCOMPTON_HH
#define NONLINEARCOMPTON_HH

#include "LaserField.hh"
#include "Particle.hh"
#include "ParticleList.hh"

class NonLinearCompton
{
public:
	NonLinearCompton(LaserField* field, double dt);
	
	~NonLinearCompton();

	// Main function carrying out the process
	void Interact(Particle &part, ParticleList *partList);

private:
	LaserField* m_filed;	// LaserField interacting with particle
	double m_dt;	// simulation time step
};

#endif