#include<string>
#include "OutputManager.hh"

OutputManager::OutputManager(HDF5Output* file):
m_outputFile(file), m_units(NULL)
{
	SetFields(true,true,true);
}

OutputManager::OutputManager(HDF5Output* file, UnitsSystem* units):
m_outputFile(file), m_units(units)
{
	SetFields(true,true,true);
}

OutputManager::~OutputManager()
{
}

void OutputManager::SetFields(bool partOn, bool fieldOn, bool qedOn)
{
	if (partOn == true)
	{
		m_outputFile->AddGroup("Particles");
	}
	if (fieldOn == true)
	{
		m_outputFile->AddGroup("Fields");
	}
	if (qedOn == true)
	{
		m_outputFile->AddGroup("Histograms");
	}
}

void OutputManager::SingleParticle(const Particle& part, std::string name)
{
	if (m_singlePartBool == false)
	{
		m_outputFile->AddGroup("Particles/Single");
		m_singlePartBool = true;
	}
	if (part.GetTracking() == false)
	{
		std::cerr << "Error: Failed to output data for particle \"" + name + "\".\n"; 
		std::cerr << "		 Tracking set to false for this particle.\n";
		exit(1);
	} else
	{
		double length, momentum, time;
		if (m_units == NULL)
		{
			length = 1;
			momentum = 1;
			time = 1;
		} else
		{
			length = m_units->RefLength();
			momentum = m_units->RefMomentum();
			time = m_units->RefTime();
		}

		double posBuff[3*part.GetPosHist().size()];
		double momBuff[3*part.GetMomHist().size()];
		double timeBuff[part.GetTimeHist().size()];
		double gammaBuff[part.GetGammaHist().size()];		
		for (unsigned int i = 0; i < part.GetPosHist().size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				posBuff[3*i+j] = part.GetPosHist()[i][j] * length;
			}
		}
		for (unsigned int i = 0; i < part.GetMomHist().size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				momBuff[3*i+j] = part.GetMomHist()[i][j] * momentum;
			}
		}
		for (unsigned int i = 0; i < part.GetTimeHist().size(); i++)
		{
			timeBuff[i] = part.GetTimeHist()[i] * time;
		}
		for (unsigned int i = 0; i < part.GetGammaHist().size(); i++)
		{
			gammaBuff[i] = part.GetGammaHist()[i];
		}
		std::string groupName = "Particles/Single/" + name;
		m_outputFile->AddGroup(groupName);
		m_outputFile->AddArray2D(posBuff, part.GetPosHist().size(), 3, groupName + "/Position");
		m_outputFile->AddArray2D(momBuff, part.GetMomHist().size(), 3, groupName + "/Momentum");
		m_outputFile->AddArray1D(timeBuff, part.GetTimeHist().size(), groupName + "/Time");
		m_outputFile->AddArray1D(gammaBuff, part.GetGammaHist().size(), groupName + "/Gamma");
	}
}

void OutputManager::ListParticle(ParticleList* partList, std::string name)
{
	if (m_partListBool == false)
	{
		m_outputFile->AddGroup("Particles/ParticleList");
		m_partListBool = true;
	}
	m_outputFile->AddGroup("Particles/ParticleList/" + name);
	for (unsigned int i = 0; i < partList->GetNPart(); i++)
	{
		Particle part = partList->GetParticle(i);
		if (part.GetTracking() == false)
		{
			std::cerr << "Error: Failed to output data for particle \"" << name 
					  << " " << i << "\".\n"; 
			std::cerr << "		 Tracking set to false for this particle.\n";
			exit(1);
		} else
		{
			double length, momentum, time;
			if (m_units == NULL)
			{
				length = 1;
				momentum = 1;
				time = 1;
			} else
			{
				length = m_units->RefLength();
				momentum = m_units->RefMomentum();
				time = m_units->RefTime();
			}

			double posBuff[3*part.GetPosHist().size()];
			double momBuff[3*part.GetMomHist().size()];
			double timeBuff[part.GetTimeHist().size()];
			double gammaBuff[part.GetGammaHist().size()];		
			for (unsigned int i = 0; i < part.GetPosHist().size(); i++)
			{
				for (unsigned int j = 0; j < 3; j++)
				{
					posBuff[3*i+j] = part.GetPosHist()[i][j] * length;
				}
			}
			for (unsigned int i = 0; i < part.GetMomHist().size(); i++)
			{
				for (unsigned int j = 0; j < 3; j++)
				{
					momBuff[3*i+j] = part.GetMomHist()[i][j] * momentum;
				}
			}
			for (unsigned int i = 0; i < part.GetTimeHist().size(); i++)
			{
				timeBuff[i] = part.GetTimeHist()[i] * time;
			}
			for (unsigned int i = 0; i < part.GetGammaHist().size(); i++)
			{
				gammaBuff[i] = part.GetGammaHist()[i];
			}
			std::string groupName = "Particles/ParticleList/" + name + "/" + std::to_string(i);
			m_outputFile->AddGroup(groupName);
			m_outputFile->AddArray2D(posBuff, part.GetPosHist().size(), 3, groupName + "/Position");
			m_outputFile->AddArray2D(momBuff, part.GetMomHist().size(), 3, groupName + "/Momentum");
			m_outputFile->AddArray1D(timeBuff, part.GetTimeHist().size(), groupName + "/Time");
			m_outputFile->AddArray1D(gammaBuff, part.GetGammaHist().size(), groupName + "/Gamma");
		}
	}
}

void OutputManager::EMField(Field* field, const std::vector<double> &tAxis,
						   				  const std::vector<double> &xAxis,
								 	      const std::vector<double> &yAxis,
									      const std::vector<double> &zAxis)
{

}

void OutputManager::OutputHist(Histogram* hist)
{
	std::string groupName = "Histograms/"+ hist->GetName();
	m_outputFile->AddGroup(groupName);
	m_outputFile->AddArray1D(hist->GetBinCentres(), hist->GetNBins(), groupName + "/BinCentres");
	m_outputFile->AddArray1D(hist->GetBinValues(), hist->GetNBins(), groupName + "/BinValues");

}