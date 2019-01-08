#include <cmath>

#include "Particle.hh"
#include "MCTools.hh"

Particle::Particle()
{
}

Particle::Particle(double mass, double charge, double time, bool tracking):
m_charge(charge), m_mass(mass), m_tracking(tracking), m_time(time)
{
	InitOpticalDepth();
}

Particle::Particle(double mass, double charge, const ThreeVector &position, 
			 	   const ThreeVector &momentum, double time, bool tracking):
m_charge(charge), m_mass(mass), m_tracking(tracking), m_time(time)
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

void Particle::SaveTrack(HDF5Output *file, std::string partName, int partIndex) const
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
		std::string groupName = "Particles/" + partName + "/part" + std::to_string(partIndex);
		file->AddGroup(groupName);
		file->AddArray2D(posBuff, m_posHistory.size(), 3, groupName + "/Position");
		file->AddArray2D(momBuff, m_momHistory.size(), 3, groupName + "/Momentum");
		file->AddArray1D(timeBuff, m_posHistory.size(), groupName + "/Time");
		file->AddArray1D(gammaBuff, m_momHistory.size(), groupName + "/Gamma");
	} else
	{
		std::cerr << "Error: Output failed. Tracking not turned on." << std::endl; 
	}
}

void Particle::InitOpticalDepth()
{
	m_opticalDepth = -1.0 * std::log(1.0 - MCTools::RandDouble(0.0, 1.0));
}