#ifndef PHOTON_HH
#define PHOTON_HH

#include "Particle.hh"

class Photon: public Particle 
{
public:
	Photon(const ThreeVector &position, const ThreeVector &momentum,
		double weight = 1, double time = 0, bool tracking = false);

	Photon(double energy, const ThreeVector &position,
		const ThreeVector &direction, double weight = 1, double time = 0,
		bool tracking = false);

	~Photon();

	double GetGamma() const override;

	double GetBeta() const override;

	double GetEnergy() const override;

	std::string GetType() const override {return "Photon";}

	std::string GetName() const override {return "Photon";}
};
#endif