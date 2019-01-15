#ifndef NONLINEARCOMPTON_HH
#define NONLINEARCOMPTON_HH

#include "LaserField.hh"
#include "Particle.hh"
#include "ParticleList.hh"
#include "UnitsSystem.hh"

class NonLinearCompton
{
public:
	NonLinearCompton(LaserField* field, UnitsSystem* units, double dt);
	
	~NonLinearCompton();

	// Main function carrying out the process
	void Interact(Particle &part, ParticleList *partList);

private:
	
	// Calculates the value of eta at the particles current location
	double CalculateEta(const Particle &part);

	double CalculateH(double eta);

	// loads the required tabulated data required for interaction. Uses log-log tables
	void LoadTables();

private:
	LaserField* m_filed;	// LaserField interacting with particle
	UnitsSystem* m_units;
	double m_dt;	// simulation time step	

	std::vector<double> m_hVector;
	std::vector<double> m_etaVector;
};

#endif