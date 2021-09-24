#include <cmath>

#include "Particle.hh"
#include "MCTools.hh"

Particle::Particle()
{
}

Particle::Particle(double mass, double charge, double weight, double time,
	bool tracking):
m_mass(mass), m_charge(charge), m_weight(weight), m_time(time),
m_tracking(tracking), m_isAlive(true)
{
	InitOpticalDepth();
}

Particle::Particle(double mass, double charge, const ThreeVector &position, 
			 	   const ThreeVector &momentum, double weight, double time,
			 	   bool tracking):
m_mass(mass), m_charge(charge), m_weight(weight), m_time(time),
m_tracking(tracking), m_isAlive(true)
{
	InitOpticalDepth();
	m_momentum = momentum;
	m_position = position;
}

Particle::~Particle()
{
}

void Particle::UpdateTrack(const ThreeVector &position, const ThreeVector &momentum)
{
	m_position = position;
	m_momentum = momentum;
	if (m_tracking == true)
	{
		m_posHistory.push_back(position);
		m_momHistory.push_back(momentum);
		m_timeHistory.push_back(m_time);
		m_gammaHistory.push_back(GetGamma());
	}
}

void Particle::UpdateTime(double dt)
{
	m_time += dt;
}

void Particle::UpdateOpticalDepth(double dtau)
{
	m_opticalDepth -= dtau;
}

ThreeVector Particle::GetDirection() const
{
	return m_momentum.Norm();
}

void Particle::InitOpticalDepth()
{
	m_opticalDepth = -1.0 * std::log(1.0 - MCTools::RandDouble(0.0, 1.0));
}