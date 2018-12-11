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

ThreeVector Particle::GetVelocity() const
{
	ThreeVector velcoity = m_momentum / (m_mass * GetGamma());
	return velcoity;
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
		double timeBuff[m_timeHistory.size()];
		double gammaBuff[m_gammaHistory.size()];		
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
		for (unsigned int i = 0; i < m_timeHistory.size(); i++)
		{
			timeBuff[i] = m_timeHistory[i];
		}
		for (unsigned int i = 0; i < m_gammaHistory.size(); i++)
		{
			gammaBuff[i] = m_gammaHistory[i];
		}
		file.AddGroup("Particle");
		file.AddArray2D(posBuff, m_posHistory.size(), 3, "Particle/Position");
		file.AddArray2D(momBuff, m_momHistory.size(), 3, "Particle/Momentum");
		file.AddArray1D(timeBuff, m_posHistory.size(), "Particle/Time");
		file.AddArray1D(gammaBuff, m_momHistory.size(), "Particle/Gamma");
	} else
	{
		std::cerr << "Error: Output failed. Tracking not turned on." << std::endl; 
	}
}