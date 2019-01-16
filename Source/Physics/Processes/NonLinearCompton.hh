#ifndef NONLINEARCOMPTON_HH
#define NONLINEARCOMPTON_HH

#include "Field.hh"
#include "Particle.hh"
#include "ParticleList.hh"
#include "UnitsSystem.hh"

class NonLinearCompton
{
public:
	NonLinearCompton(Field* field, UnitsSystem* units, double dt);
	
	~NonLinearCompton();

	// Main function carrying out the process. The particle is the iunterafting particle
	// and the particle list is where the new particle will be added.
	void Interact(Particle &part, ParticleList *partList);

public:
	
	// Calculates the value of eta at the particles current location
	double CalculateEta(const Particle &part);

	double CalculateH(double eta);

	double SampleChi(double eta);

	void LoadTables();

	void UnloadTables();

private:
	Field* m_filed;	// LaserField interacting with particle
	UnitsSystem* m_units;
	double m_dt;	// simulation time step	

	// Data tables for h factor
	double* m_h_dataTable;
	double* m_h_etaAxis;
	unsigned int m_h_length;


	// Data tables used for calculating the photon energy
	double** m_phEn_dataTable;
	double** m_phEn_chiAxis;
	double* m_phEn_chiMinAxis; 	
	double* m_phEn_etaAxis;
	unsigned int m_phEn_etaLength, m_phEn_chiLength;
};
#endif