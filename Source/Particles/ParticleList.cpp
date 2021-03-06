#include "ParticleList.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"
#include "Lepton.hh"
#include "Photon.hh"
#include <fstream>

ParticleList::ParticleList(std::string name, unsigned int maxParticles):
m_name(name), m_maxParticles(maxParticles), m_particleNumber(0)
{
	m_particleList = std::vector<Particle*>(m_maxParticles);
}

ParticleList::~ParticleList()
{
	for (unsigned int i = 0; i < m_particleNumber; i++)
	{
		delete m_particleList[i];
	}
}

void ParticleList::AddParticle(Particle *part)
{
	if (m_particleNumber < m_maxParticles)
	{
		m_particleList[m_particleNumber] = part;
		m_particleNumber++;
	} else
	{
		std::cerr << "Error: Particle list too small!\n";
		std::cerr << "New particles will be forgotten\n";
	}
}