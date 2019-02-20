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
}

void ParticleList::AddParticle(Particle *part)
{
	m_particleList[m_particleNumber] = part;
	m_particleNumber++;
}

void ParticleList::MonoSource(std::string type, unsigned int nPart, double energy,
							  double deltaPos, const ThreeVector &position,
							  const ThreeVector &direction)
{
	m_particleNumber = nPart;
	std::vector<double> rPos = MCTools::SampleNorm(0, deltaPos, nPart);
	ThreeMatrix m_rotaion = direction.RotateToAxis(ThreeVector(0, 0, 1));
	for (unsigned int i = 0; i < nPart; i++)
	{
		double angle = MCTools::RandDouble(0, 2.0 * UnitsSystem::pi);
		ThreeVector partPosition = ThreeVector(std::cos(angle) * rPos[i], 
											   std::sin(angle) * rPos[i], 0.0);
		partPosition = m_rotaion * partPosition + position;

		if (type == "Photon" || type == "photon")
		{
			Photon* part = new Photon(energy, partPosition, direction.Norm());
			m_particleList[i] = part;
		} else if (type == "Electron" || type == "electron")
		{
			Lepton* part = new Lepton(1.0, -1.0, energy, partPosition, direction.Norm());
			m_particleList[i] = part;
		} else if (type == "Positron" || type == "positron")
		{
			Lepton* part = new Lepton(1.0, 1.0, energy, partPosition, direction.Norm());
			m_particleList[i] = part;
		} else
		{
			std::cerr << "Error: Unkown particle type: " << type << "\n";
			std::cerr << "Exiting!\n";
			exit(-1);
		}
	}
}

void ParticleList::LinearSource(std::string type, unsigned int nPart, double energyMin,
								double energyMax, double deltaPos, const ThreeVector &position,
								const ThreeVector &direction)
{
	m_particleNumber = nPart;
	std::vector<double> rPos = MCTools::SampleNorm(0, deltaPos, nPart);
	ThreeMatrix m_rotaion = direction.RotateToAxis(ThreeVector(0, 0, 1));
	for (unsigned int i = 0; i < nPart; i++)
	{
		double angle = MCTools::RandDouble(0, 2.0 * UnitsSystem::pi);
		ThreeVector partPosition = ThreeVector(std::cos(angle) * rPos[i], 
											   std::sin(angle) * rPos[i], 0.0);
		partPosition = m_rotaion * partPosition + position;
		double energy = MCTools::RandDouble(energyMin, energyMax);

		if (type == "Photon" || type == "photon")
		{
			Photon* part = new Photon(energy, partPosition, direction.Norm());
			m_particleList[i] = part;
		} else if (type == "Electron" || type == "electron")
		{
			Lepton* part = new Lepton(1.0, -1.0, energy, partPosition, direction.Norm());
			m_particleList[i] = part;
		} else if (type == "Positron" || type == "positron")
		{
			Lepton* part = new Lepton(1.0, 1.0, energy, partPosition, direction.Norm());
			m_particleList[i] = part;
		} else
		{
			std::cerr << "Error: Unkown particle type: " << type << "\n";
			std::cerr << "Exiting!\n";
			exit(-1);
		}
	}
}