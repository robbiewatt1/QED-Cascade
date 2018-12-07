#include <cmath>

#include "Particle.hh"

Particle::Particle()
{
}

Particle::Particle(double mass, double charge, bool tracking):
m_charge(charge), m_mass(mass), m_tracking(tracking), m_time(0)
{
}

Particle::~Particle()
{
}

Particle::Particle(double mass, double charge, const ThreeVector &position, 
			 	   const ThreeVector &momentum, bool tracking):
m_charge(charge), m_mass(mass), m_tracking(tracking), m_time(0)
{
	m_momentum = momentum;
	m_position = position;
}

void Particle::UpdatePosition(const ThreeVector &position)
{
	m_position = position;
	if (m_tracking == true)
	{
		m_posHistory.push_back(position);
	}
}

void Particle::UpdateMomentum(const ThreeVector &momentum)
{
	m_momentum = momentum;
	if (m_tracking == true)
	{
		m_momHistory.push_back(momentum);
	}
}

double Particle::GetGamma() const
{
	if (m_mass > 0)
	{
		return std::sqrt(1.0 + m_momentum.Mag2() / (m_mass * m_mass));
	} else
	{
		return 0;
	}
}

void Particle::SaveTrack(HDF5Output &file) const
{
	if (m_tracking == true)
	{
		double posBuff[3*m_posHistory.size()];
		double momBuff[3*m_momHistory.size()];
		for (unsigned int i = 0; i < m_posHistory.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				posBuff[3*i+j] = m_posHistory[i][j];
			}
		}
		for (unsigned int i = 0; i < m_momHistory.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				momBuff[3*i+j] = m_momHistory[i][j];
			}
		}
		file.AddArray2D(posBuff, m_posHistory.size(), 3, "Position");
		file.AddArray2D(momBuff, m_momHistory.size(), 3, "Momentum");
	} else
	{
		std::cerr << "Error: Output failed. Tracking not turned on." << std::endl; 
	}
}