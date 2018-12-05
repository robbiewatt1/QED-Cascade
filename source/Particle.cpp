#include <cmath>

#include "Particle.hh"

Particle::Particle()
{
}

Particle::Particle(double mass, double charge, bool tracking):
m_charge(charge), m_mass(mass), m_tracking(tracking)
{
}

Particle::Particle(double mass, double charge, const std::vector<double> &position, 
			 	   const std::vector<double> &momentum, bool tracking):
m_charge(charge), m_mass(mass), m_tracking(tracking)
{
	m_momentum = momentum;
	m_position = position;
}

void Particle::UpdatePosition(const std::vector<double> &position)
{
	m_position = position;
	if (m_tracking == true)
	{
		m_posHistory.push_back(position);
	}
}

void Particle::UpdateMomentum(const std::vector<double> &momentum)
{
	m_momentum = momentum;
	if (m_tracking == true)
	{
		m_momHistory.push_back(momentum);
	}
}

double Particle::GetGamma()
{
	if (m_mass > 0)
	{
		return std::sqrt(1 + (m_momentum[0] * m_momentum[0] + m_momentum[1] * m_momentum[1] 
							+ m_momentum[2] * m_momentum[2]) / (m_mass * m_mass));
	} else
	{
		return 0;
	}
}