#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "ThreeVector.hh"

class Particle
{
public:
	
	Particle();

	Particle(double mass, double charge, bool tracking = false);

	Particle(double mass, double charge, ThreeVector &position, 
			 const ThreeVector &momentum, bool tracking = false);
	
	virtual ~Particle() = 0;
	
	ThreeVector GetPosition() const {return m_position;}

	ThreeVector GetMomentum() const {return m_momentum;}

	void UpdatePosition(const ThreeVector &position);
	
	void UpdateMomentum(const ThreeVector &velocity);

	double GetMass() const {return m_mass;}

	double GetCharge() const {return m_charge;}

	double GetGamma() const;

private:
	bool m_tracking;	// If set to true, particle tracking turned on
	double m_mass;	//mass normilised to electron mass
	double m_charge;	//charge normilised to -electron charge

	ThreeVector m_position;	// current position of particle
	ThreeVector m_momentum;	// current velcoity of particle
	std::vector<ThreeVector> m_posHistory;	// Only used if tracking is on
	std::vector<ThreeVector> m_momHistory;	// only used if tracking is on
};

#endif