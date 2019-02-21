#include "Lepton.hh"

Lepton::Lepton(double mass, double charge, const ThreeVector &position, 
			   const ThreeVector &momentum, double time, bool tracking):
Particle(mass, charge, position, momentum, time, tracking)
{
	if (charge == -1)
	{
		m_name = "electron";
	} else
	{
		m_name = "positron";
	}
}

Lepton::Lepton(double mass, double charge, double energy, const ThreeVector &position,
			   const ThreeVector &direction, double time, bool tracking):
Particle(mass, charge, position, energy * direction.Norm(), time, tracking)
{
	if (charge == -1)
	{
		m_name = "electron";
	} else
	{
		m_name = "positron";
	}
}

Lepton::~Lepton()
{
}

double Lepton::GetGamma() const
{
	return std::sqrt(1.0 + m_momentum.Mag2() / (m_mass * m_mass));
}

double Lepton::GetEnergy() const
{
	return m_mass * GetGamma();
}

double Lepton::GetBeta() const
{
	return std::sqrt(1.0 - 1.0 / (GetGamma() * GetGamma()));
}

