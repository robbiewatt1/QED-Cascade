#include "SourceGenerator.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"
#include "Photon.hh"
#include "Lepton.hh"

SourceGenerator::SourceGenerator(std::string type, std::string distro,
								 unsigned int nPart, double energy1, 
								 double energy2, double deltaPos,
								 const ThreeVector &position,
								 const ThreeVector &direction,
								 bool track):
m_type(type), m_nPart(nPart), m_partCount(0), m_track(track)
{
	m_direction = direction.Norm();
	m_position = position;
	m_radialPos = MCTools::SampleNorm(0, deltaPos, nPart);
	m_angle = MCTools::SampleUniform(0, 2.0 * UnitsSystem::pi, nPart);
	if (distro == "exp")
	{
		m_energy = MCTools::SampleNorm(energy1, energy2, nPart);
	} else if(distro == "linear")
	{
		m_energy = MCTools::SampleUniform(energy1, energy2, nPart);
	} else
	{
		m_energy = MCTools::SampleUniform(energy1, energy2, nPart);
	}
	
	m_rotaion = m_direction.RotateToAxis(ThreeVector(0, 0, 1));
}

SourceGenerator::~SourceGenerator()
{

}

ParticleList* SourceGenerator::GenerateList()
{
	ParticleList* list = new ParticleList(std::to_string(m_partCount));

	ThreeVector partPosition = ThreeVector(std::cos(m_angle[m_partCount])
										   * m_radialPos[m_partCount],
										   std::sin(m_angle[m_partCount])
										   * m_radialPos[m_partCount], 0.0);
	partPosition = m_rotaion * partPosition + m_position;

	if (m_type == "Photon" || m_type == "photon")
	{
		Photon* part = new Photon(m_energy[m_partCount], partPosition, m_direction, 0, m_track);
		list->AddParticle(part);
	} else if (m_type == "Electron" || m_type == "electron")
	{
		Lepton* part = new Lepton(1.0, -1.0, m_energy[m_partCount], partPosition, m_direction, 0,
				m_track);
		list->AddParticle(part);
	} else if (m_type == "Positron" || m_type == "positron")
	{
		Lepton* part = new Lepton(1.0, 1.0, m_energy[m_partCount], partPosition, m_direction, 0,
				m_track);
		list->AddParticle(part);
	} else
	{
		std::cerr << "Error: Unkown particle type: " << m_type << "\n";
		std::cerr << "Exiting!\n";
		exit(-1);
	}
	m_partCount++;
	return list;
}
/*
ParticleList* SourceGenerator::LinearSource()
{
	std::vector<double> rPos = MCTools::SampleNorm(0, deltaPos, nPart);
	ThreeMatrix m_rotaion = direction.RotateToAxis(ThreeVector(0, 0, 1));

	ParticleList* list = new ParticleList(std::to_string(nPart));
	double angle = MCTools::RandDouble(0, 2.0 * UnitsSystem::pi);
	ThreeVector partPosition = ThreeVector(std::cos(angle) * rPos[i], 
										   std::sin(angle) * rPos[i], 0.0);
	partPosition = m_rotaion * partPosition + position;
	double energy = MCTools::RandDouble(energyMin, energyMax);

	if (type == "Photon" || type == "photon")
	{
		Photon* part = new Photon(energy, partPosition, direction.Norm());
		list->AddParticle(part);
	} else if (type == "Electron" || type == "electron")
	{
		Lepton* part = new Lepton(1.0, -1.0, energy, partPosition, direction.Norm());
		list->AddParticle(part);
	} else if (type == "Positron" || type == "positron")
	{
		Lepton* part = new Lepton(1.0, 1.0, energy, partPosition, direction.Norm());
		list->AddParticle(part);
	} else
	{
		std::cerr << "Error: Unkown particle type: " << type << "\n";
		std::cerr << "Exiting!\n";
		exit(-1);
	}
	return list;
}
*/
void SourceGenerator::FreeSources(ParticleList* source)
{
	delete source;
}