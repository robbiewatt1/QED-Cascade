#include<string>
#include "OutputManager.hh"

OutputManager::OutputManager(std::string fileName):
m_units(NULL)
{
	m_outputFile = new HDF5Output(fileName);
	m_outputFile->AddGroup("Particles");
	m_outputFile->AddGroup("Fields");
	m_outputFile->AddGroup("Histograms");
}

OutputManager::OutputManager(std::string fileName, UnitsSystem* units):
m_units(units)
{
	m_outputFile = new HDF5Output(fileName);
	m_outputFile->AddGroup("Particles");
	m_outputFile->AddGroup("Fields");
	m_outputFile->AddGroup("Histograms");
}

OutputManager::~OutputManager()
{
	delete m_outputFile;

	for (unsigned int i = 0; i < m_primaryEvent.size(); i++)
	{
		delete [] m_primaryEvent[i];
		delete [] m_electronEvent[i];
		delete [] m_positronEvent[i];
		delete [] m_photonEvent[i];
	}
}

void OutputManager::SingleParticle(Particle* part, std::string name)
{
	if (m_singlePartBool == false)
	{
		m_outputFile->AddGroup("Particles/Single");
		m_singlePartBool = true;
	}
	if (part->GetTracking() == false)
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

		double posBuff[3*part->GetPosHist().size()];
		double momBuff[3*part->GetMomHist().size()];
		double timeBuff[part->GetTimeHist().size()];
		double gammaBuff[part->GetGammaHist().size()];		
		for (unsigned int i = 0; i < part->GetPosHist().size(); i++)
		{
			ThreeVector pos = part->GetPosHist()[i];
			ThreeVector mom = part->GetMomHist()[i];
			posBuff[3*i]   = pos[0] * length;
			posBuff[3*i+1] = pos[1] * length;
			posBuff[3*i+2] = pos[2] * length;
			momBuff[3*i]   = mom[0] * momentum;
			momBuff[3*i+1] = mom[1] * momentum;
			momBuff[3*i+2] = mom[2] * momentum;
		}
		for (unsigned int i = 0; i < part->GetTimeHist().size(); i++)
		{
			timeBuff[i] = part->GetTimeHist()[i] * time;
		}
		for (unsigned int i = 0; i < part->GetGammaHist().size(); i++)
		{
			gammaBuff[i] = part->GetGammaHist()[i];
		}
		std::string groupName = "Particles/Single/" + name;
		m_outputFile->AddGroup(groupName);
		m_outputFile->AddArray2D(posBuff, part->GetPosHist().size(), 3, groupName + "/Position");
		m_outputFile->AddArray2D(momBuff, part->GetMomHist().size(), 3, groupName + "/Momentum");
		m_outputFile->AddArray1D(timeBuff, part->GetTimeHist().size(), groupName + "/Time");
		m_outputFile->AddArray1D(gammaBuff, part->GetGammaHist().size(), groupName + "/Gamma");
	}
}

void OutputManager::ListTracks(ParticleList* partList, std::string name)
{
	if (m_partListBool == false)
	{
		m_outputFile->AddGroup("Particles/ParticleList");
		m_partListBool = true;
	}
	m_outputFile->AddGroup("Particles/ParticleList/" + name);
	for (unsigned int i = 0; i < partList->GetNPart(); i++)
	{
		Particle* part = partList->GetParticle(i);
		if (part->GetTracking() == false)
		{
			std::cerr << "Error: Failed to output data for particle \"" << name 
					  << " " << i << "\".\n"; 
			std::cerr << "Tracking set to false for this particle.\n";
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

			double posBuff[3*part->GetPosHist().size()];
			double momBuff[3*part->GetMomHist().size()];
			double timeBuff[part->GetTimeHist().size()];
			double gammaBuff[part->GetGammaHist().size()];		
			for (unsigned int i = 0; i < part->GetPosHist().size(); i++)
			{
				for (unsigned int j = 0; j < 3; j++)
				{
					posBuff[3*i+j] = part->GetPosHist()[i][j] * length;
				}
			}
			for (unsigned int i = 0; i < part->GetMomHist().size(); i++)
			{
				for (unsigned int j = 0; j < 3; j++)
				{
					momBuff[3*i+j] = part->GetMomHist()[i][j] * momentum;
				}
			}
			for (unsigned int i = 0; i < part->GetTimeHist().size(); i++)
			{
				timeBuff[i] = part->GetTimeHist()[i] * time;
			}
			for (unsigned int i = 0; i < part->GetGammaHist().size(); i++)
			{
				gammaBuff[i] = part->GetGammaHist()[i];
			}
			std::string groupName = "Particles/ParticleList/" + name + "/" + std::to_string(i);
			m_outputFile->AddGroup(groupName);
			m_outputFile->AddArray2D(posBuff, part->GetPosHist().size(), 3, groupName + "/Position");
			m_outputFile->AddArray2D(momBuff, part->GetMomHist().size(), 3, groupName + "/Momentum");
			m_outputFile->AddArray1D(timeBuff, part->GetTimeHist().size(), groupName + "/Time");
			m_outputFile->AddArray1D(gammaBuff, part->GetGammaHist().size(), groupName + "/Gamma");
		}
	}
}

void OutputManager::ListProperties(ParticleList* partList, std::string setName)
{
	if (m_partListBool == false)
	{
		m_outputFile->AddGroup("Particles/ParticleList");
		m_partListBool = true;
	}
	std::string groupName = "Particles/ParticleList/" + partList->GetName();
	m_outputFile->AddGroup(groupName);
	
	double* dataBuff = new double[partList->GetNPart()*5];
	for (unsigned int i = 0; i < partList->GetNPart(); i++)
	{
		dataBuff[i*5] = i;
		dataBuff[i*5+1] = partList->GetParticle(i)->GetEnergy();
		dataBuff[i*5+2] = partList->GetParticle(i)->GetPosition()[0];
		dataBuff[i*5+3] = partList->GetParticle(i)->GetPosition()[1];
		dataBuff[i*5+4] = partList->GetParticle(i)->GetPosition()[2];
	}
	m_outputFile->AddArray2D(dataBuff, partList->GetNPart(), 5, groupName + "/" + setName);
}

void OutputManager::InitSource(unsigned int nEvents)
{
	m_primaryEvent  = std::vector<double*>(nEvents);
	m_electronEvent = std::vector<double*>(nEvents);
	m_positronEvent = std::vector<double*>(nEvents);
	m_photonEvent   = std::vector<double*>(nEvents);
	m_electronCount = std::vector<unsigned int>(nEvents);
	m_positronCount = std::vector<unsigned int>(nEvents);
	m_photonCount   = std::vector<unsigned int>(nEvents);
}

void OutputManager::StoreSource(ParticleList* partList, unsigned int eventID, bool primary)
{
	if (primary == true)
	{
		double* dataBuff = new double[8];
		dataBuff[0] = eventID;
		dataBuff[1] = partList->GetParticle(0)->GetEnergy(),
		dataBuff[2] = partList->GetParticle(0)->GetMomentum()[0],
		dataBuff[3] = partList->GetParticle(0)->GetMomentum()[1],
		dataBuff[4] = partList->GetParticle(0)->GetMomentum()[2],
		dataBuff[5] = partList->GetParticle(0)->GetPosition()[0],
		dataBuff[6] = partList->GetParticle(0)->GetPosition()[1],
		dataBuff[7] = partList->GetParticle(0)->GetPosition()[2];
		m_primaryEvent[eventID] = dataBuff;
	} else
	{
		m_electronCount[eventID] = 0;
		m_positronCount[eventID] = 0;
		m_photonCount[eventID] = 0;
		for (unsigned int i = 0; i < partList->GetNPart(); i++)
		{
			if (partList->GetParticle(i)->GetName() == "Electron") m_electronCount[eventID]++;
			if (partList->GetParticle(i)->GetName() == "Positron") m_positronCount[eventID]++;
			if (partList->GetParticle(i)->GetName() == "Photon") m_photonCount[eventID]++;
		}
		double* electronData = new double [m_electronCount[eventID]*8];
		double* positronData = new double [m_positronCount[eventID]*8];
		double* photonData   = new double [m_photonCount[eventID]*8];
		m_electronCount[eventID] = 0;
		m_positronCount[eventID] = 0;
		m_photonCount[eventID] = 0;
		for (unsigned int i = 0; i < partList->GetNPart(); i++)
		{
			if (partList->GetParticle(i)->GetName() == "Electron")
			{
				electronData[m_electronCount[eventID]*8]   = eventID;
				electronData[m_electronCount[eventID]*8+1] = partList->GetParticle(i)
						->GetEnergy();
				electronData[m_electronCount[eventID]*8+2] = partList->GetParticle(i)
						->GetMomentum()[0];
				electronData[m_electronCount[eventID]*8+3] = partList->GetParticle(i)
						->GetMomentum()[1];
				electronData[m_electronCount[eventID]*8+4] = partList->GetParticle(i)
						->GetMomentum()[2];
				electronData[m_electronCount[eventID]*8+5] = partList->GetParticle(i)
						->GetPosition()[0];
				electronData[m_electronCount[eventID]*8+6] = partList->GetParticle(i)
						->GetPosition()[1];
				electronData[m_electronCount[eventID]*8+7] = partList->GetParticle(i)
						->GetPosition()[2];
				m_electronCount[eventID]++;
			} else if (partList->GetParticle(i)->GetName() == "Positron")
			{
				positronData[m_positronCount[eventID]*8]   = eventID;
				positronData[m_positronCount[eventID]*8+1] = partList->GetParticle(i)
						->GetEnergy();
				positronData[m_positronCount[eventID]*8+2] = partList->GetParticle(i)
						->GetMomentum()[0];
				positronData[m_positronCount[eventID]*8+3] = partList->GetParticle(i)
						->GetMomentum()[1];
				positronData[m_positronCount[eventID]*8+4] = partList->GetParticle(i)
						->GetMomentum()[2];
				positronData[m_positronCount[eventID]*8+5] = partList->GetParticle(i)
						->GetPosition()[0];
				positronData[m_positronCount[eventID]*8+6] = partList->GetParticle(i)
						->GetPosition()[1];
				positronData[m_positronCount[eventID]*8+7] = partList->GetParticle(i)
						->GetPosition()[2];
				m_positronCount[eventID]++;
			}  else if (partList->GetParticle(i)->GetName() == "Photon")
			{
				photonData[m_photonCount[eventID]*8]   = eventID;
				photonData[m_photonCount[eventID]*8+1] = partList->GetParticle(i)
						->GetEnergy();
				photonData[m_photonCount[eventID]*8+2] = partList->GetParticle(i)
						->GetMomentum()[0];
				photonData[m_photonCount[eventID]*8+3] = partList->GetParticle(i)
						->GetMomentum()[1];
				photonData[m_photonCount[eventID]*8+4] = partList->GetParticle(i)
						->GetMomentum()[2];
				photonData[m_photonCount[eventID]*8+5] = partList->GetParticle(i)
						->GetPosition()[0];
				photonData[m_photonCount[eventID]*8+6] = partList->GetParticle(i)
						->GetPosition()[1];
				photonData[m_photonCount[eventID]*8+7] = partList->GetParticle(i)
						->GetPosition()[2];
				m_photonCount[eventID]++;
			}
		}
		m_electronEvent[eventID] = electronData;
		m_positronEvent[eventID] = positronData;
		m_photonEvent[eventID]   = photonData;
	}
}

void OutputManager::StoreTrack(ParticleList* partList, unsigned int eventID)
{
	for (unsigned int i = 0; i < partList->GetNPart(); i++)
	{

		m_idTrack.push_back(eventID);
		m_positionTrack.push_back(partList->GetParticle(i)->GetPosHist());
		m_momentumTrack.push_back(partList->GetParticle(i)->GetMomHist());
		m_timeTrack.push_back(partList->GetParticle(i)->GetTimeHist());
		m_gammaTrack.push_back(partList->GetParticle(i)->GetGammaHist());
	}
}

void OutputManager::OutputEvents(bool outSource, bool outTrack)
{
	if (outSource == true)
	{
		if (m_particleSourceBool == false)
		{
			m_outputFile->AddGroup("Particles/ParticleSource");
			m_particleSourceBool = true;
		}

		double* primeBuff = new double [m_primaryEvent.size()*8];
		for (unsigned int i = 0; i < m_primaryEvent.size(); i++)
		{
			primeBuff[i*8]   = m_primaryEvent[i][0];
			primeBuff[i*8+1] = m_primaryEvent[i][1];
			primeBuff[i*8+2] = m_primaryEvent[i][2];
			primeBuff[i*8+3] = m_primaryEvent[i][3];
			primeBuff[i*8+4] = m_primaryEvent[i][4];
			primeBuff[i*8+5] = m_primaryEvent[i][5];
			primeBuff[i*8+6] = m_primaryEvent[i][6];
			primeBuff[i*8+7] = m_primaryEvent[i][7];
		}
		m_outputFile->AddArray2D(primeBuff, m_primaryEvent.size(), 8,
				"Particles/ParticleSource/Primary");

		unsigned int totalElectrons = 0;
		unsigned int totalPositrons = 0;
		unsigned int totalPhotons = 0;

		for (unsigned int i = 0; i < m_electronCount.size(); i++)
		{
			totalElectrons += m_electronCount[i];
			totalPositrons += m_positronCount[i];
			totalPhotons   += m_photonCount[i];
		}

		double* electronBuff = new double [totalElectrons*8];
		double* positronBuff = new double [totalPositrons*8];
		double* photonBuff   = new double [totalPhotons*8];
		unsigned int electronIndex = 0;
		unsigned int psoitronIndex = 0;
		unsigned int photonIndex = 0;

		for (unsigned int i = 0; i < m_electronEvent.size(); i++)
		{
			for (unsigned int j = 0; j < m_electronCount[i]; j++)
			{
				electronBuff[electronIndex]   = m_electronEvent[i][j*8];
				electronBuff[electronIndex+1] = m_electronEvent[i][j*8+1];
				electronBuff[electronIndex+2] = m_electronEvent[i][j*8+2];
				electronBuff[electronIndex+3] = m_electronEvent[i][j*8+3];
				electronBuff[electronIndex+4] = m_electronEvent[i][j*8+4];
				electronBuff[electronIndex+5] = m_electronEvent[i][j*8+5];
				electronBuff[electronIndex+6] = m_electronEvent[i][j*8+6];
				electronBuff[electronIndex+7] = m_electronEvent[i][j*8+7];
				electronIndex += 8;
			}
			for (unsigned int j = 0; j < m_positronCount[i]; j++)
			{
				positronBuff[psoitronIndex]   = m_positronEvent[i][j*8];
				positronBuff[psoitronIndex+1] = m_positronEvent[i][j*8+1];
				positronBuff[psoitronIndex+2] = m_positronEvent[i][j*8+2];
				positronBuff[psoitronIndex+3] = m_positronEvent[i][j*8+3];
				positronBuff[psoitronIndex+4] = m_positronEvent[i][j*8+4];
				positronBuff[psoitronIndex+5] = m_positronEvent[i][j*8+5];
				positronBuff[psoitronIndex+6] = m_positronEvent[i][j*8+6];
				positronBuff[psoitronIndex+7] = m_positronEvent[i][j*8+7];
				psoitronIndex += 8;
			}
			for (unsigned int j = 0; j < m_photonCount[i]; j++)
			{
				photonBuff[photonIndex]   = m_photonEvent[i][j*8];
				photonBuff[photonIndex+1] = m_photonEvent[i][j*8+1];
				photonBuff[photonIndex+2] = m_photonEvent[i][j*8+2];
				photonBuff[photonIndex+3] = m_photonEvent[i][j*8+3];
				photonBuff[photonIndex+4] = m_photonEvent[i][j*8+4];
				photonBuff[photonIndex+5] = m_photonEvent[i][j*8+5];
				photonBuff[photonIndex+6] = m_photonEvent[i][j*8+6];
				photonBuff[photonIndex+7] = m_photonEvent[i][j*8+7];
				photonIndex += 8;
			}				
		}
		if (totalElectrons > 0)
		{
			m_outputFile->AddArray2D(electronBuff, totalElectrons, 8,
					"Particles/ParticleSource/Electrons");
		}
		if (totalPositrons > 0)
		{
			m_outputFile->AddArray2D(positronBuff, totalPositrons, 8,
					"Particles/ParticleSource/Positrons");
		}
		if (totalPhotons > 0)
		{
			m_outputFile->AddArray2D(photonBuff, totalPhotons, 8,
					"Particles/ParticleSource/Photons");
		}
		delete [] primeBuff;
		delete [] electronBuff;
		delete [] positronBuff;
		delete [] photonBuff;
	}

	// Output tracking info
	if (outTrack == true)
	{
		if (m_partListBool == false)
		{
			m_outputFile->AddGroup("Particles/ParticleTrack");
			m_partListBool = true;
		}

		// Get the units system
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

		// Loop through all the tracks
		for (unsigned int i = 0; i < m_idTrack.size(); i++)
		{
			std::string eventName = "Particles/ParticleTrack/Event" + std::to_string(i);
			m_outputFile->AddGroup(eventName);

			double posBuff[3*m_positionTrack[i].size()];
			double momBuff[3*m_momentumTrack[i].size()];
			double timeBuff[m_timeTrack[i].size()];
			double gammaBuff[m_gammaTrack[i].size()];		
			for (unsigned int j = 0; j < m_positionTrack[i].size(); j++)
			{
				for (unsigned int k = 0; k < 3; k++)
				{
					posBuff[3*j+k] = m_positionTrack[i][j][k] * length;
				}
			}
			for (unsigned int j = 0; j < m_momentumTrack[i].size(); j++)
			{
				for (unsigned int k = 0; k < 3; k++)
				{
					momBuff[3*j+k] = m_momentumTrack[i][j][k] * momentum;
				}
			}
			for (unsigned int j = 0; j < m_timeTrack[i].size(); j++)
			{
				timeBuff[j] = m_timeTrack[i][j] * time;
			}
			for (unsigned int j = 0; j < m_gammaTrack[i].size(); j++)
			{
				gammaBuff[j] = m_gammaTrack[i][j] * time;
			}
			m_outputFile->AddArray2D(posBuff, m_positionTrack[i].size(), 3, eventName + "/Position");
			m_outputFile->AddArray2D(momBuff, m_momentumTrack[i].size(), 3, eventName + "/Momentum");
			m_outputFile->AddArray1D(timeBuff, m_timeTrack[i].size(), eventName + "/Time");
			m_outputFile->AddArray1D(gammaBuff, m_gammaTrack[i].size(), eventName + "/Gamma");
		}
	}
}

void OutputManager::OutEMField(EMField* field, const std::vector<double> &tAxis,
						   	   const std::vector<double> &xAxis,
							   const std::vector<double> &yAxis,
							   const std::vector<double> &zAxis)
{
	for (unsigned int t = 0; t < tAxis.size(); t++)
	{
		std::string groupName = "Fields/" + std::to_string(tAxis[t]);
		groupName.erase(groupName.find_last_not_of('0') + 1, std::string::npos);
		m_outputFile->AddGroup(groupName);
		for (int dir = 0; dir < 3; dir++)
		{
			double* dataBuff = new double[xAxis.size()*yAxis.size()*zAxis.size()];
			for (unsigned int i = 0; i < xAxis.size(); i++)
			{
				for (unsigned int j = 0; j < yAxis.size(); j++)
				{
					for (unsigned int k = 0; k < zAxis.size(); k++)
					{
						unsigned int index = (i*yAxis.size()*zAxis.size()) + (j*zAxis.size()) + k;
						ThreeVector efield, bfield;
						field->GetField(tAxis[t], ThreeVector(xAxis[i], yAxis[j], zAxis[k]),
									   efield, bfield);
						dataBuff[index] = efield[dir];
					}
				}
			}
			std::string dataName = groupName + "/E" + std::to_string(dir);
			m_outputFile->AddArray3D(dataBuff, xAxis.size(), yAxis.size(), zAxis.size(), dataName);
			delete [] dataBuff;
		}
	}
}

void OutputManager::OutputHist(Histogram* hist)
{
	std::string groupName = "Histograms/"+ hist->GetName();
	m_outputFile->AddGroup(groupName);
	m_outputFile->AddArray1D(hist->GetBinCentres(), hist->GetNBins(), groupName + "/BinCentres");
	m_outputFile->AddArray1D(hist->GetBinValues(), hist->GetNBins(), groupName + "/BinValues");

}