#include<string>
#include "OutputManager.hh"

OutputManager::OutputManager(std::string fileName):
m_units(NULL)
{
#ifdef USEMPI
    // Only open on master
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    if(id == 0){
#endif
    m_outputFile = new HDF5Output(fileName);
    m_outputFile->AddGroup("Particles");
    m_outputFile->AddGroup("Fields");
    m_outputFile->AddGroup("Histograms");
#ifdef USEMPI
    }
#endif
}

OutputManager::OutputManager(std::string fileName, UnitsSystem* units):
m_units(units)
{
#ifdef USEMPI
    // Only open on master
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    if(id == 0){
#endif
    m_outputFile = new HDF5Output(fileName);
    m_outputFile->AddGroup("Particles");
    m_outputFile->AddGroup("Fields");
    m_outputFile->AddGroup("Histograms");
#ifdef USEMPI
    }
#endif
}

OutputManager::~OutputManager()
{
#ifdef USEMPI
    // Only close on master
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    if(id == 0){
#endif
    delete m_outputFile;
#ifdef USEMPI
    }
#endif
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
        std::cerr << "       Tracking set to false for this particle.\n";
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
        m_photonCount[eventID]   = 0;
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
            double* dataBuffE = new double[xAxis.size()*yAxis.size()*zAxis.size()];
            double* dataBuffB = new double[xAxis.size()*yAxis.size()*zAxis.size()];
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
                        dataBuffE[index] = efield[dir];
                        dataBuffB[index] = bfield[dir];
                    }
                }
            }
            std::string dataNameE = groupName + "/E" + std::to_string(dir);
            std::string dataNameB = groupName + "/B" + std::to_string(dir);
            m_outputFile->AddArray3D(dataBuffE, xAxis.size(), yAxis.size(), zAxis.size(), dataNameE);
            m_outputFile->AddArray3D(dataBuffB, xAxis.size(), yAxis.size(), zAxis.size(), dataNameB);
            delete [] dataBuffE;
            delete [] dataBuffB;
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

#ifdef USEMPI
void OutputManager::OutputEventsMPI(bool outSource, bool outTrack)
{
    int primaryTag = 1;
    int electronTag = 2;
    int positronTag = 3;
    int photonTag = 4;
    // check if master or slave
    int id, nProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    if (outSource == true)
    {
        if (id == 0)
        {
            // We are master
            if (m_particleSourceBool == false)
            {
                m_outputFile->AddGroup("Particles/ParticleSource");
                m_particleSourceBool = true;
            }

            // First start with primary sources
            int arraysize[nProc];
            int totalSize = m_primaryEvent.size() * 8;
            // We need to know the size of the arrays that we are recieving
            for (int i = 1; i < nProc; ++i)
            {
                MPI_Status status;
                MPI_Probe(i, primaryTag, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_DOUBLE, &arraysize[i]);
                totalSize += arraysize[i];
            }
            // Now create the buffer array;
            double* primeBuff = new double [totalSize];

            // Fill the prime buff with own data
            for (long unsigned int i = 0; i < m_primaryEvent.size(); i++)
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

            int primeBuffIndex = m_primaryEvent.size() * 8;
            // Now fill with other
            for (int i = 1; i < nProc; ++i)
            {
                double* procBuff = new double [arraysize[i]];
                MPI_Status status;
                MPI_Recv(procBuff, arraysize[i], MPI_DOUBLE, i,
                    primaryTag, MPI_COMM_WORLD, &status);
                for (int j = 0; j < arraysize[i] / 8; j++)
                {
                    primeBuff[primeBuffIndex]   = procBuff[j*8];
                    primeBuff[primeBuffIndex+1] = procBuff[j*8+1];
                    primeBuff[primeBuffIndex+2] = procBuff[j*8+2];
                    primeBuff[primeBuffIndex+3] = procBuff[j*8+3];
                    primeBuff[primeBuffIndex+4] = procBuff[j*8+4];
                    primeBuff[primeBuffIndex+5] = procBuff[j*8+5];
                    primeBuff[primeBuffIndex+6] = procBuff[j*8+6];
                    primeBuff[primeBuffIndex+7] = procBuff[j*8+7];
                    primeBuffIndex +=8;
                }
                delete[] procBuff;
            }
            m_outputFile->AddArray2D(primeBuff, totalSize / 8, 8,
                "Particles/ParticleSource/Primary");
            delete[] primeBuff;

            // Now we move on the end events
            long unsigned int totalElectrons = 0;
            long unsigned int totalPositrons = 0;
            long unsigned int totalPhotons = 0;
            for (unsigned int i = 0; i < m_electronCount.size(); i++)
            {
                totalElectrons += m_electronCount[i];
                totalPositrons += m_positronCount[i];
                totalPhotons   += m_photonCount[i];
            }

            // Now do the same for slaves
            int electronArraysize[nProc];
            int positronArraysize[nProc];
            int photonArraysize[nProc];
            int totalelectronSize = totalElectrons * 8;
            int totalpositronSize = totalPositrons * 8;
            int totalphotonSize = totalPhotons * 8;
            for (int i = 1; i < nProc; ++i)
            {
                MPI_Status status;
                MPI_Recv(&electronArraysize[i], 1, MPI_INT, i, electronTag,
                    MPI_COMM_WORLD, &status);
                totalelectronSize += electronArraysize[i];
                MPI_Recv(&positronArraysize[i], 1, MPI_INT, i, positronTag,
                    MPI_COMM_WORLD, &status);
                totalpositronSize += positronArraysize[i];
                MPI_Recv(&photonArraysize[i], 1, MPI_INT, i, photonTag,
                    MPI_COMM_WORLD, &status);
                totalphotonSize += photonArraysize[i];
            }

            double* electronBuff = new double [totalelectronSize];
            double* positronBuff = new double [totalpositronSize];
            double* photonBuff   = new double [totalphotonSize];
            long unsigned int electronIndex = 0;
            long unsigned int positronIndex = 0;
            long unsigned int photonIndex = 0;

            // Start filling with master data
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
                    positronBuff[positronIndex]   = m_positronEvent[i][j*8];
                    positronBuff[positronIndex+1] = m_positronEvent[i][j*8+1];
                    positronBuff[positronIndex+2] = m_positronEvent[i][j*8+2];
                    positronBuff[positronIndex+3] = m_positronEvent[i][j*8+3];
                    positronBuff[positronIndex+4] = m_positronEvent[i][j*8+4];
                    positronBuff[positronIndex+5] = m_positronEvent[i][j*8+5];
                    positronBuff[positronIndex+6] = m_positronEvent[i][j*8+6];
                    positronBuff[positronIndex+7] = m_positronEvent[i][j*8+7];
                    positronIndex += 8;
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
            
            // Now start filling up the array with slave data
            for (int i = 1; i < nProc; ++i)
            {
                double* procElectronBuff = new double [electronArraysize[i]];
                MPI_Status statusE;
                MPI_Recv(procElectronBuff, electronArraysize[i], MPI_DOUBLE,
                    i, electronTag, MPI_COMM_WORLD, &statusE);
                for (int j = 0; j < electronArraysize[i] / 8; j++)
                {
                    electronBuff[electronIndex]   = procElectronBuff[j*8];
                    electronBuff[electronIndex+1] = procElectronBuff[j*8+1];
                    electronBuff[electronIndex+2] = procElectronBuff[j*8+2];
                    electronBuff[electronIndex+3] = procElectronBuff[j*8+3];
                    electronBuff[electronIndex+4] = procElectronBuff[j*8+4];
                    electronBuff[electronIndex+5] = procElectronBuff[j*8+5];
                    electronBuff[electronIndex+6] = procElectronBuff[j*8+6];
                    electronBuff[electronIndex+7] = procElectronBuff[j*8+7];
                    electronIndex +=8;
                }
                delete[] procElectronBuff;

                double* procPositronBuff = new double [positronArraysize[i]];
                MPI_Status statusP;
                MPI_Recv(procPositronBuff, positronArraysize[i], MPI_DOUBLE, i,
                    positronTag, MPI_COMM_WORLD, &statusP);
                for (int j = 0; j < positronArraysize[i] / 8; j++)
                {
                    positronBuff[positronIndex]   = procPositronBuff[j*8];
                    positronBuff[positronIndex+1] = procPositronBuff[j*8+1];
                    positronBuff[positronIndex+2] = procPositronBuff[j*8+2];
                    positronBuff[positronIndex+3] = procPositronBuff[j*8+3];
                    positronBuff[positronIndex+4] = procPositronBuff[j*8+4];
                    positronBuff[positronIndex+5] = procPositronBuff[j*8+5];
                    positronBuff[positronIndex+6] = procPositronBuff[j*8+6];
                    positronBuff[positronIndex+7] = procPositronBuff[j*8+7];
                    positronIndex +=8;
                }
                delete[] procPositronBuff;

                double* procPhotonnBuff  = new double [photonArraysize[i]];
                MPI_Status statusG;
                MPI_Recv(procPhotonnBuff, photonArraysize[i], MPI_DOUBLE, i,
                    photonTag, MPI_COMM_WORLD, &statusG);
                for (int j = 0; j < photonArraysize[i] / 8; j++)
                {
                    photonBuff[photonIndex]   = procPhotonnBuff[j*8];
                    photonBuff[photonIndex+1] = procPhotonnBuff[j*8+1];
                    photonBuff[photonIndex+2] = procPhotonnBuff[j*8+2];
                    photonBuff[photonIndex+3] = procPhotonnBuff[j*8+3];
                    photonBuff[photonIndex+4] = procPhotonnBuff[j*8+4];
                    photonBuff[photonIndex+5] = procPhotonnBuff[j*8+5];
                    photonBuff[photonIndex+6] = procPhotonnBuff[j*8+6];
                    photonBuff[photonIndex+7] = procPhotonnBuff[j*8+7];
                    photonIndex +=8;
                }
                delete[] procPhotonnBuff;
            }
            if (totalelectronSize > 0)
            {
                m_outputFile->AddArray2D(electronBuff, totalelectronSize / 8, 8,
                        "Particles/ParticleSource/Electrons");
            }
            if (totalpositronSize > 0)
            {
                m_outputFile->AddArray2D(positronBuff, totalpositronSize / 8, 8,
                        "Particles/ParticleSource/Positrons");
            }
            if (totalphotonSize > 0)
            {
                m_outputFile->AddArray2D(photonBuff, totalphotonSize / 8, 8,
                        "Particles/ParticleSource/Photons");
            }
            delete [] electronBuff;
            delete [] positronBuff;
            delete [] photonBuff;

            if(outTrack == true)
            {
                std::cerr << "Warnimg: Tracking source is not available when"
                             "using MPI." << std::endl;
            }

        } else
        {
            // We are slave
            // Start with the primary source
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
            // Sed over primary buff data
            MPI_Send(primeBuff, m_primaryEvent.size() * 8, MPI_DOUBLE, 0,
                primaryTag, MPI_COMM_WORLD);
            delete[] primeBuff;

            // Not send over final data

            // Get the total number of each partile
            // Now we move on the end events
            long unsigned int totalElectrons = 0;
            long unsigned int totalPositrons = 0;
            long unsigned int totalPhotons = 0;
            for (unsigned int i = 0; i < m_electronCount.size(); i++)
            {
                totalElectrons += m_electronCount[i];
                totalPositrons += m_positronCount[i];
                totalPhotons   += m_photonCount[i];
            }
            double* electronBuff = new double[totalElectrons*8];
            double* positronBuff = new double[totalPositrons*8];
            double* photonBuff   = new double[totalPhotons*8];

            long unsigned int electronIndex = 0;
            long unsigned int psoitronIndex = 0;
            long unsigned int photonIndex = 0;
            // Start filling the buffer arrays
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
        // Send over array sizes
        long unsigned int electronSize = totalElectrons * 8;
        long unsigned int positronSize = totalPositrons * 8;
        long unsigned int photonSize = totalPhotons * 8;
        MPI_Send(&electronSize, 1, MPI_INT, 0, electronTag,
            MPI_COMM_WORLD);
        MPI_Send(&positronSize, 1, MPI_INT, 0, positronTag,
            MPI_COMM_WORLD);
        MPI_Send(&photonSize, 1, MPI_INT, 0, photonTag,
            MPI_COMM_WORLD);
        // Sed over arrays
        MPI_Send(electronBuff, electronSize, MPI_DOUBLE, 0, electronTag,
            MPI_COMM_WORLD);
        MPI_Send(positronBuff, positronSize, MPI_DOUBLE, 0, positronTag,
            MPI_COMM_WORLD);
        MPI_Send(photonBuff, photonSize, MPI_DOUBLE, 0, photonTag, 
            MPI_COMM_WORLD);
        delete[] electronBuff;
        delete[] positronBuff;
        delete[] photonBuff;
        }
    }
}

void OutputManager::OutputHistMPI(Histogram* hist)
{
    // Collect all histograms from other processors are sum them. They will all
    // have the same bin centres so just need to send over the bin values

    // check if master or slave
    int histTag = 0;
    int id, nProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    if(id == 0)
    {
        // We are master,
        double* binValues = new double [hist->GetNBins()];
        double* dataBuff = new double [hist->GetNBins()];

        // Start by adding our own
        for(unsigned int i = 0; i < hist->GetNBins(); i++)
        {
            binValues[i] = hist->GetBinValues()[i];
        }

        // Now add slaves
        for (int rank = 1; rank < nProc; rank++)
        {
            MPI_Status status;
            MPI_Recv(dataBuff, hist->GetNBins(), MPI_DOUBLE, MPI_ANY_SOURCE,
                histTag, MPI_COMM_WORLD ,&status);
            // Sum bins
            for(unsigned int i = 0; i < hist->GetNBins(); i++)
            {
                binValues[i] += dataBuff[i];
            }
        }

        // Now save to the file
        std::string groupName = "Histograms/"+ hist->GetName();
        m_outputFile->AddGroup(groupName);
        m_outputFile->AddArray1D(hist->GetBinCentres(), hist->GetNBins(),
            groupName + "/BinCentres");
        m_outputFile->AddArray1D(binValues, hist->GetNBins(),
            groupName + "/BinValues");

        delete[] binValues;
        delete[] dataBuff;
    } else
    {
        // We are slave
        MPI_Send(hist->GetBinValues(), hist->GetNBins(), MPI_DOUBLE, 0, histTag,
            MPI_COMM_WORLD);
    }
    // Wait for everyone to finish
    MPI_Barrier(MPI_COMM_WORLD);
}
#endif