#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "ThreeVector.hh"
#include "HDF5Output.hh"

class Particle
{
public:
	
	Particle();

	Particle(double mass, double charge, double time = 0, bool tracking = false);

	Particle(double mass, double charge, const ThreeVector &position, 
			 const ThreeVector &momentum, double time = 0, bool tracking = false);
	
	~Particle();
	
	void UpdateTrack(const ThreeVector &position, const ThreeVector &momentum);
	
	void UpdateTime(double dt);

	ThreeVector GetPosition() const {return m_position;}

	ThreeVector GetMomentum() const {return m_momentum;}

	ThreeVector GetVelocity() const;

	double GetMass() const {return m_mass;}

	double GetCharge() const {return m_charge;}

	double GetTime() const {return m_time;}

	double GetGamma() const;

	void SaveTrack(HDF5Output &file) const;

private:
	bool m_tracking;	// If set to true, particle tracking turned on
	double m_mass;	//mass normilised to electron mass
	double m_charge;	//charge normilised to -electron charge
	double m_time;	// global time of particle

	ThreeVector m_position;	// current position of particle
	ThreeVector m_momentum;	// current velcoity of particle
	std::vector<ThreeVector> m_posHistory;	// Only used if tracking is on
	std::vector<ThreeVector> m_momHistory;	// only used if tracking is on
	std::vector<double> m_timeHistory;
	std::vector<double> m_gammaHistory;

};

#endif