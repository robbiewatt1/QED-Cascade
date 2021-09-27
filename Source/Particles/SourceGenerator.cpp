#include "SourceGenerator.hh"
#include "MCTools.hh"
#include "UnitsSystem.hh"
#include "Photon.hh"
#include "Lepton.hh"

SourceGenerator::SourceGenerator(std::string type, std::string distro,
                                 unsigned int nPart, double energy1, 
                                 double energy2, double deltaPos,
                                 double deltaTau, double deltaDir,
                                 const ThreeVector &position,
                                 const ThreeVector &direction,
                                 bool track):
m_type(type), m_nPart(nPart), m_partCount(0), m_track(track)
{
    m_direction = direction.Norm();
    m_position  = position;
    m_xPos = MCTools::SampleNorm(0, deltaPos, nPart);
    m_yPos = MCTools::SampleNorm(0, deltaPos, nPart);
    m_zPos = MCTools::SampleNorm(0, deltaTau, nPart);
    m_thetaDir = MCTools::SampleNorm(0, deltaDir, nPart);
    m_phiDir = MCTools::SampleUniform(0, 2.0 * UnitsSystem::pi, nPart);
    if (distro == "gaussian" || distro == "Gaussian")
    {
        m_energy = MCTools::SampleNorm(energy1, energy2, nPart);
    } else if(distro == "linear" || distro == "Linear")
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
    if (m_partCount == m_nPart)
    {
        std::cerr << "Error: Particle source depleted." << std::endl;
        std::exit(1);
    }
    ParticleList* list = new ParticleList(std::to_string(m_partCount));

    ThreeVector partPosition = ThreeVector(m_xPos[m_partCount],
                                           m_yPos[m_partCount],
                                           m_zPos[m_partCount]);
    partPosition = m_rotaion * partPosition + m_position;

    ThreeVector partDirection = ThreeVector(std::sin(m_thetaDir[m_partCount])
                                          * std::cos(m_phiDir[m_partCount]),
                                            std::sin(m_thetaDir[m_partCount])
                                          * std::sin(m_phiDir[m_partCount]),
                                            std::cos(m_thetaDir[m_partCount]));
    partDirection = m_rotaion * partDirection;

    if (m_type == "Photon" || m_type == "photon")
    {
        Photon* part = new Photon(m_energy[m_partCount], partPosition,
            partDirection, 1, 0, m_track);
        list->AddParticle(part);
    } else if (m_type == "Electron" || m_type == "electron")
    {
        Lepton* part = new Lepton(1.0, -1.0, m_energy[m_partCount],
            partPosition, partDirection, 1, 0, m_track);
        list->AddParticle(part);
    } else if (m_type == "Positron" || m_type == "positron")
    {
        Lepton* part = new Lepton(1.0, 1.0, m_energy[m_partCount],
            partPosition, partDirection, 1, 0, m_track);
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

void SourceGenerator::FreeSources(ParticleList* source)
{
    delete source;
}