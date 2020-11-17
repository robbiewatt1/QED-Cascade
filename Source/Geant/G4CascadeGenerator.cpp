#include "G4CascadeGenerator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"


#include "LorentzPusher.hh"
#include "LandauPusher.hh"
#include "ModifiedLandauPusher.hh"

#include "GaussianEMField.hh"
#include "StaticEMField.hh"
#include "PlaneEMField.hh"
#include "FocusingField.hh"
#include "NonLinearCompton.hh"
#include "NonLinearBreitWheeler.hh"
#include "FileParser.hh"


G4CascadeGenerator::G4CascadeGenerator(std::string iniPath,
    G4ThreeVector& position):
G4VUserPrimaryGeneratorAction(), m_currentEvent(0), m_currentSecondary(-1)
{
    // set up particle gun properties
    m_particleGun = new G4ParticleGun();
    m_particleGun->SetParticlePosition(position);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    m_electronDef = particleTable->FindParticle("e-");
    m_positronDef = particleTable->FindParticle("e+");
    m_gammaDef = particleTable->FindParticle("gamma");

    // Load QED-Cascade classes
    m_units = new UnitsSystem("SI");
    FileParser* parser = new FileParser(iniPath, true);
    GeneralParameters inGeneral = parser->GetGeneral();
    FieldParameters inField = parser->GetField();
    ProcessParameters inProcess = parser->GetProcess();
    ParticleParameters inParticles = parser->GetParticle()[0];

    m_timeStep = inGeneral.timeStep;
    m_timeEnd = inGeneral.timeEnd;

    // Set up the m_fields
    if (inField.Type == "static")
    {
        m_field = new StaticEMField(inField.E, inField.B);
    } else if (inField.Type == "plane")
    {
        m_field = new PlaneEMField(inField.MaxE, inField.Wavelength,
            inField.Polerisation, inField.Direction);
    } else if (inField.Type == "gaussian")
    {
        m_field = new GaussianEMField(inField.MaxE, inField.Wavelength,
            inField.Duration, inField.Waist, inField.Polerisation,
            inField.Start, inField.Focus);
    } else if (inField.Type == "focusing")
    {
        m_field = new FocusingField(inField.MaxE, inField.Wavelength,
            inField.Duration, inField.Waist, inField.Polerisation,
            inField.Start, inField.Focus);
    } else
    {
        std::cerr << "Error: unknown m_field type." << std::endl;
        exit(-1);
    }

    // Set up the physics pusher
    if (inGeneral.pusher == "Lorentz")
    {
        m_pusher = new LorentzPusher(m_field, inGeneral.timeStep);
    } else if (inGeneral.pusher == "Landau")
    {
        m_pusher = new LandauPusher(m_field, inGeneral.timeStep);
    } else if (inGeneral.pusher == "ModifiedLandau")
    {
        m_pusher = new ModifiedLandauPusher(m_field, inGeneral.timeStep);
    } else
    {
        std::cerr << "Error: Unkown Pusher Type" << std::endl;
        exit(-1);
    }

    // set up the particle sources
    m_source = new SourceGenerator(inParticles.Type,
        inParticles.Distro, inParticles.Number,
        inParticles.Energy1, inParticles.Energy2, inParticles.Radius,
        inParticles.Duration, inParticles.Divergence, inParticles.Position,
        inParticles.Direction, false);

    // Set up the Physics list
    if (inProcess.NonLinearCompton == true)
    {
        NonLinearCompton* comptonNL = new NonLinearCompton(m_field,
            inGeneral.timeStep, inGeneral.tracking, inGeneral.minEnergy);
        m_processList.push_back(comptonNL);
    }
    if (inProcess.NonLinearBreitWheeler == true)
    {
        NonLinearBreitWheeler* breitWheelerNL = new NonLinearBreitWheeler(m_field, 
                                                            inGeneral.timeStep,
                                                            inGeneral.tracking);
        m_processList.push_back(breitWheelerNL);
    }
}

G4CascadeGenerator::~G4CascadeGenerator()
{
    delete m_source;
    delete m_units;
    delete m_field;
    delete m_pusher;
}

void G4CascadeGenerator::SimulateEvent()
{
    // Free resource
    if(m_currentEvent != 0)
    {
        m_source->FreeSources(m_event);
    }
    m_event = m_source->GenerateList();
    double time = 0;

    while(time < m_timeEnd) // loop time
    {
        // Push particles and interact
        for (unsigned int i = 0; i < m_event->GetNPart(); i++) // Loop particles
        {
            m_pusher->PushParticle(m_event->GetParticle(i));
            for (unsigned int proc = 0; proc < m_processList.size(); proc++) // loop processes
            {
                m_processList[proc]->Interact(m_event->GetParticle(i), m_event);
            }
        }
        time += m_timeStep;  
    }
    m_currentSecondary = m_event->GetNPart() - 1;
    m_currentEvent++;
}

void G4CascadeGenerator::GeneratePrimaries(G4Event* anEvent)
{
    // Abort run once we have ran out of particles
    if (m_currentEvent == m_source->GetSourceNumber()
        && m_currentSecondary == -1)
    {
        G4RunManager* manager = G4RunManager::GetRunManager();
        manager->AbortRun();
    } else
    {
        // Check for particle in the list
        if (m_currentSecondary == -1)
        {
            SimulateEvent();
        }
        // Load the gun params
        if(m_event->GetParticle(m_currentSecondary)->GetName() == "Electron")
        {  
            m_particleGun->SetParticleDefinition(m_electronDef);
        } else if (m_event->GetParticle(m_currentSecondary)->GetName()
            == "Positron")
        {
            m_particleGun->SetParticleDefinition(m_positronDef);
        } else
        {
            m_particleGun->SetParticleDefinition(m_gammaDef);
        }

         m_particleGun->SetParticleEnergy(
            m_event->GetParticle(m_currentSecondary)->GetEnergy()
                                / m_units->G4Energy());

         ThreeVector dir = m_event->GetParticle(m_currentSecondary)
            ->GetDirection();

        m_particleGun->SetParticleMomentumDirection(
            G4ThreeVector(dir[0], dir[1], dir[2]));
        m_currentSecondary--;

        m_particleGun->GeneratePrimaryVertex(anEvent);
    }
}