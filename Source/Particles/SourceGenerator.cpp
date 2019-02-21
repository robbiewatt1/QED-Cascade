#include "SourceGenerator.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"
#include "Photon.hh"
#include "Lepton.hh"

std::vector<ParticleList*> SourceGenerator::MonoSource(std::string type, unsigned int nPart, 
													   double energy, double deltaPos, 
													   const ThreeVector &position,
							  						   const ThreeVector &direction)
{
	std::vector<ParticleList*> sourceList(nPart);
	std::vector<double> rPos = MCTools::SampleNorm(0, deltaPos, nPart);
	ThreeMatrix m_rotaion = direction.RotateToAxis(ThreeVector(0, 0, 1));
	for (unsigned int i = 0; i < nPart; i++)
	{
		ParticleList* list = new ParticleList(std::to_string(nPart));
		double angle = MCTools::RandDouble(0, 2.0 * UnitsSystem::pi);
		ThreeVector partPosition = ThreeVector(std::cos(angle) * rPos[i], 
											   std::sin(angle) * rPos[i], 0.0);
		partPosition = m_rotaion * partPosition + position;

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
		sourceList[i] = list;
	}
	return sourceList;
}

std::vector<ParticleList*> SourceGenerator::LinearSource(std::string type, unsigned int nPart, 
														 double energyMin, double energyMax, 
														 double deltaPos, const ThreeVector &position,
														 const ThreeVector &direction)
{
	std::vector<ParticleList*> sourceList(nPart);
	std::vector<double> rPos = MCTools::SampleNorm(0, deltaPos, nPart);
	ThreeMatrix m_rotaion = direction.RotateToAxis(ThreeVector(0, 0, 1));
	for (unsigned int i = 0; i < nPart; i++)
	{
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
		sourceList[i] = list;
	}
	return sourceList;
}

void SourceGenerator::FreeSources(std::vector<ParticleList*> source)
{
	for (unsigned int i = 0; i < source.size(); i++)
	{
		delete source[i];
	}
}