#include <algorithm>
#include "FileParser.hh"
#include "UnitsSystem.hh"
#include <fstream>

FileParser::FileParser(std::string fileName, bool checkOutput):
m_checkOutput(checkOutput)
{
    m_reader = new INIReader(fileName);
    m_sections = m_reader->GetSections();
    CheckVitals();

    if (m_checkOutput == true)
    {
        m_checkFile.open("./input-check.txt");
        if(!m_checkFile)
        {
            std::cerr << "Error: Cannot open input-check.txt" << std::endl;
        }
    }
    ReadGeneral();
    ReadField();
    ReadParticles();
    ReadPhysics();
    ReadHistograms();
}

FileParser::~FileParser()
{
    if (m_checkOutput == true)
    {
        m_checkFile.close();
    }
    delete m_reader;
    delete m_units;
}

void FileParser::CheckVitals()
{
    if (std::find(m_sections.begin(), m_sections.end(), "General") == m_sections.end())
    {
        std::cerr << "Input error: File does not contain \"General\" field\n";
        std::cerr << "Exiting!\n";
        exit(1);
    } else if (!(m_reader->HasValue("General", "time_step")))
    {
        std::cerr << "Input error: File does not contain \"time_step\" field\n";
        std::cerr << "Exiting!\n";
        exit(1);
    } else if (!(m_reader->HasValue("General", "time_end")))
    {
        std::cerr << "Input error: File does not contain \"time_end\" field\n";
        std::cerr << "Exiting!\n";
        exit(1);
    } else if (!(m_reader->HasValue("General", "units")))
    {
        std::cerr << "Input error: File does not contain \"Units\" field\n";
        std::cerr << "Exiting!\n";
        exit(1);
    }
}

void FileParser::ReadGeneral()
{
    m_general.units = m_reader->GetString("General", "units", "None");
    m_units = new UnitsSystem(m_general.units);
    m_general.timeStep = m_reader->GetReal("General", "time_step", 0) / m_units->RefTime();
    m_general.timeEnd = m_reader->GetReal("General", "time_end", 0) / m_units->RefTime();
    m_general.fileName = m_reader->GetString("General", "file_name", "out.h5");
    m_general.tracking = m_reader->GetBoolean("General", "tracking", false);

    if (m_checkOutput == true)
    {
        m_checkFile << "General input parameters \n";
        m_checkFile << "Units       = " << m_general.units << "\n";
        m_checkFile << "Time step   = " << m_general.timeStep << "\n";
        m_checkFile << "Time end    = " << m_general.timeEnd << "\n";
        m_checkFile << "Output file = " << m_general.fileName << "\n";
        m_checkFile << "Tracking    = " << m_general.tracking << "\n";
        m_checkFile << "\n\n";
    }
}

void FileParser::ReadField()
{
    if (std::find(m_sections.begin(), m_sections.end(), "Field") != m_sections.end())
    {
        m_field.Type = m_reader->GetString("Field", "field_type", "");
        if (m_field.Type != "gaussian" && m_field.Type != "plane"
            && m_field.Type != "static" && m_field.Type != "focusing")
         {
            std::cerr << "Input error: Field type \"" << m_field.Type << "\" is not recognise.\n";
            std::cerr << "Exiting!\n";
            exit(1);
         } 
         if (m_field.Type == "static")
         {
            m_field.E = m_reader->GetThreeVector("Field", "e_field", ThreeVector(0,0,0)) 
                                                                       / m_units->RefEField();
            m_field.B = m_reader->GetThreeVector("Field", "b_field", ThreeVector(0,0,0))
                                                                      / m_units->RefBField();
            if (m_checkOutput == true)
            {
                m_checkFile << "Field input parameters \n";
                m_checkFile << "Field type = " << m_field.Type << "\n";
                m_checkFile << "E strength = [" << m_field.E[0] << " " << m_field.E[1]
                            << " " << m_field.E[2] << "]\n";
                m_checkFile << "B strength = [" << m_field.B[0] << " " << m_field.B[1]
                            << " " << m_field.B[2] << "]\n";
                m_checkFile << "\n\n";
            }
         } else if (m_field.Type == "plane")
         {
            m_field.MaxE = m_reader->GetReal("Field", "e_max", 0) / m_units->RefEField();
            m_field.Wavelength = m_reader->GetReal("Field", "wavelength", 1) / m_units->RefLength();
            m_field.Direction = m_reader->GetThreeVector("Field", "direction", ThreeVector(0,0,1));
            m_field.Polerisation = m_reader->GetReal("Field", "polerisation", 0);
            if (m_checkOutput == true)
            {
                m_checkFile << "Field input parameters \n";
                m_checkFile << "Field type  = " << m_field.Type << "\n";
                m_checkFile << "Wavelength  = " << m_field.Wavelength << "\n";
                m_checkFile << "Direction   = [" << m_field.Direction[0] << " "
                            << m_field.Direction[1] << " " << m_field.Direction[2] << "]\n";
                m_checkFile << "Polerisation = " << m_field.Polerisation << "\n";
                m_checkFile << "\n\n";
            }
         } else if (m_field.Type == "gaussian")
         {
            m_field.MaxE = m_reader->GetReal("Field", "e_max", 0) / m_units->RefEField();
            m_field.Wavelength = m_reader->GetReal("Field", "wavelength", 1) / m_units->RefLength();
            m_field.Duration = m_reader->GetReal("Field", "duration", 1) / m_units->RefTime();
            m_field.Waist = m_reader->GetReal("Field", "waist", 1) / m_units->RefLength();
            m_field.Polerisation = m_reader->GetReal("Field", "polerisation", 0);
            m_field.Start = m_reader->GetThreeVector("Field", "start", ThreeVector(0,0,0)) 
                                                                    / m_units->RefLength();
            m_field.Focus = m_reader->GetThreeVector("Field", "focus", ThreeVector(0,0,0))
                                                                    / m_units->RefLength();
            if (m_checkOutput == true)
            {
                m_checkFile << "Field input parameters \n";
                m_checkFile << "Field type  = "   << m_field.Type << "\n";
                m_checkFile << "Max E       = "   << m_field.MaxE << "\n";
                m_checkFile << "Duration    = "   << m_field.Duration << "\n";
                m_checkFile << "Waist       = "   << m_field.Waist << "\n";             
                m_checkFile << "Wavelength  = "   << m_field.Wavelength << "\n";
                m_checkFile << "Start       = ["  << m_field.Start[0] << " "
                            << m_field.Start[1]   << " " << m_field.Start[2] << "]\n";
                m_checkFile << "Focus       = ["  << m_field.Focus[0] << " "
                            << m_field.Focus[1]   << " " << m_field.Focus[2] << "]\n";
                m_checkFile << "\n\n";
            }                                                                   
         } else if (m_field.Type == "focusing")
         {
            m_field.MaxE = m_reader->GetReal("Field", "e_max", 0) / m_units->RefEField();
            m_field.Wavelength = m_reader->GetReal("Field", "wavelength", 1) / m_units->RefLength();
            m_field.Duration = m_reader->GetReal("Field", "duration", 1) / m_units->RefTime();
            m_field.Waist = m_reader->GetReal("Field", "waist", 1) / m_units->RefLength();
            m_field.Polerisation = m_reader->GetReal("Field", "polerisation", 0);
            m_field.Start = m_reader->GetThreeVector("Field", "start", ThreeVector(0,0,0)) 
                                                                    / m_units->RefLength();
            m_field.Focus = m_reader->GetThreeVector("Field", "focus", ThreeVector(0,0,0))
                                                                    / m_units->RefLength();
            if (m_checkOutput == true)
            {
                m_checkFile << "Field input parameters \n";
                m_checkFile << "Field type  = "   << m_field.Type << "\n";
                m_checkFile << "Max E       = "   << m_field.MaxE << "\n";
                m_checkFile << "Duration    = "   << m_field.Duration << "\n";
                m_checkFile << "Waist       = "   << m_field.Waist << "\n";             
                m_checkFile << "Wavelength  = "   << m_field.Wavelength << "\n";
                m_checkFile << "Start       = ["  << m_field.Start[0] << " "
                            << m_field.Start[1]   << " " << m_field.Start[2] << "]\n";
                m_checkFile << "Focus       = ["  << m_field.Focus[0] << " "
                            << m_field.Focus[1]   << " " << m_field.Focus[2] << "]\n";
                m_checkFile << "\n\n";
            }                                                                   
         }
    }
}

void FileParser::ReadPhysics()
{

    m_physics.Physics = m_reader->GetString("Physics", "radiation_model", "");
    m_physics.MinEnergy = m_reader->GetReal("Physics", "min_energy", 0) / m_units->RefEnergy();
    m_physics.SampleFraction = m_reader->GetReal("Physics", "sample_fraction", 1);
    m_physics.PairProduction = m_reader->GetBoolean("Physics", "pair_production", false);

    if (m_checkOutput == true)
    {
        m_checkFile << "Physics parameters \n";
        m_checkFile << "Physics module used   = " << m_physics.Physics << "\n";
        m_checkFile << "Pair production       = " << m_physics.PairProduction << "\n";
        m_checkFile << "Sample fraction       = " << m_physics.SampleFraction << "\n";
        m_checkFile << "Minimum photon energy = " << m_physics.MinEnergy << "\n";
        m_checkFile << "\n\n";
    }
}

void FileParser::ReadParticles()
{
    unsigned int i(1);
    while(true)
    {
        std::string partField = "Particle" + std::to_string(i);
        if (std::find(m_sections.begin(), m_sections.end(), partField) 
             != m_sections.end())
        {
            ParticleParameters source;
            source.Number   = m_reader->GetInteger(partField, "number_particles", 1);
            source.Name     = m_reader->GetString(partField, "name", partField);
            source.Type     = m_reader->GetString(partField, "particle_type", "Electron");
            source.Distro   = m_reader->GetString(partField, "energy_distribution", "mono");
            source.Position = m_reader->GetThreeVector(partField, "position", ThreeVector(0, 0, 0))
                                                                             / m_units->RefLength();
            source.Direction = m_reader->GetThreeVector(partField, "direction", ThreeVector(0, 0, 1));
            source.Energy1 = m_reader->GetReal(partField, "energy_mean", 0) / m_units->RefEnergy();
            source.Energy2 = m_reader->GetReal(partField, "energy_sig", 0) / m_units->RefEnergy();
            source.Radius    = m_reader->GetReal(partField, "radius", 0) / m_units->RefLength();
            source.Divergence    = m_reader->GetReal(partField, "divergence", 0);
            source.Duration  = m_reader->GetReal(partField, "duration", 0) / m_units->RefTime();
            source.Output    = m_reader->GetBoolean(partField, "output", false);
            m_particles.push_back(source);
            i++;
            if (m_checkOutput == true)
            {
                m_checkFile << "Particle " << i-1 <<  "\n";
                m_checkFile << "Name       = " << source.Name << "\n";              
                m_checkFile << "Number     = " << source.Number << "\n";
                m_checkFile << "Type       = " << source.Type << "\n";
                m_checkFile << "Energy1    = " << source.Energy1 << "\n";
                m_checkFile << "Energy2    = " << source.Energy2 << "\n";
                m_checkFile << "Radius     = " << source.Radius << "\n";
                m_checkFile << "Divergence     = " << source.Divergence << "\n";
                m_checkFile << "Duration   = " << source.Duration << "\n";
                m_checkFile << "Output     = " << source.Output << "\n";
                m_checkFile << "Direction  = [" << source.Direction[0] << " "
                            << source.Direction[1] << " " << source.Direction[2] << "]\n";
                m_checkFile << "Position   = [" << source.Position[0] << " "
                            << source.Position[1] << " " << source.Position[2] << "]\n";
                m_checkFile << "\n\n";
            }
        } else
        {
            if (i == 1)
            {
                std::cerr << "Input warning: No particles set.\n";
                std::cerr << "This wont take long!\n";
            }
            break;
        }
    }
}

void FileParser::ReadHistograms()
{
    unsigned int i(1);
    while(true)
    {
        std::string histField = "Histogram" + std::to_string(i);
        if (std::find(m_sections.begin(), m_sections.end(), histField) 
             != m_sections.end())
        {
            HistogramParameters histogram;
            histogram.Name = m_reader->GetString(histField, "name", histField);
            histogram.Particle = m_reader->GetString(histField, "particle", "");
            histogram.Type = m_reader->GetString(histField, "type", "");
            histogram.Time = m_reader->GetReal(histField, "time", 0) / m_units->RefTime();
            histogram.Bins = m_reader->GetInteger(histField, "bins", 1);
            histogram.MinBin = m_reader->GetReal(histField, "min_bin", 0);
            histogram.MaxBin = m_reader->GetReal(histField, "max_bin", 1);
            m_histograms.push_back(histogram);
            i++;

            if (m_checkOutput == true)
            {
                m_checkFile << "Histogram " << i-1 <<  "\n";
                m_checkFile << "Name     = " << histogram.Name << "\n";
                m_checkFile << "Particle = " << histogram.Particle << "\n";
                m_checkFile << "Time     = " << histogram.Time << "\n";
                m_checkFile << "Bins     = " << histogram.Bins << "\n";
                m_checkFile << "Min Bin  = " << histogram.MinBin << "\n";
                m_checkFile << "Max Bin  = " << histogram.MaxBin << "\n";
                m_checkFile << "\n\n";
            }
        } else
        {
            break;
        }
    }
}