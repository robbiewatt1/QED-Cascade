#include <algorithm>
#include "FileParser.hh"
#include "UnitsSystem.hh"

FileParser::FileParser(std::string fileName)
{
	m_reader = new INIReader(fileName);
	m_sections = m_reader->GetSections();
	CheckVitals();

	ReadGeneral();
	ReadField();
	ReadParticles();
	ReadHistograms();
}

FileParser::~FileParser()
{
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

}

void FileParser::ReadField()
{
	if (std::find(m_sections.begin(), m_sections.end(), "Field") != m_sections.end())
	{
		m_field.Type = m_reader->GetString("Field", "field_type", "");
		if (m_field.Type != "gaussian" && m_field.Type != "plane"
			&& m_field.Type != "static")
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
		 } else if (m_field.Type == "plane")
		 {
		 	m_field.MaxE = m_reader->GetReal("Field", "e_max", 0) / m_units->RefEField();
		 	m_field.Wavelength = m_reader->GetReal("Field", "wavelength", 1) / m_units->RefLength();
		 	m_field.Direction = m_reader->GetThreeVector("Field", "direction", ThreeVector(0,0,1));
		 	m_field.Polerisation = m_reader->GetReal("Field", "polerisation", 0);
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
		 }
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
			source.Number = m_reader->GetInteger(partField, "number_particles", 1);
			source.Name = m_reader->GetString(partField, "name", partField);
			source.Type = m_reader->GetString(partField, "particle_type", "electron");
			source.Distro = m_reader->GetString(partField, "energy_distrobution", "mono");
			source.Position = m_reader->GetThreeVector(partField, "position", ThreeVector(0, 0, 0))
																			 / m_units->RefLength();
			source.Direction = m_reader->GetThreeVector(partField, "direction", ThreeVector(0, 0, 1));
			source.Energy = m_reader->GetReal(partField, "energy", 0) / m_units->RefEnergy();
			source.EnergyMin = m_reader->GetReal(partField, "min_energy", 0) / m_units->RefEnergy();
			source.EnergyMax = m_reader->GetReal(partField, "max_energy", 0) / m_units->RefEnergy();

			source.Radius = m_reader->GetReal(partField, "radius", 0) / m_units->RefLength();
			m_particles.push_back(source);
			i++;
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
		} else
		{
			break;
		}
	}
}