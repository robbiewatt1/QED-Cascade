#include "Histogram.hh"
#include "Numerics.hh"

Histogram::Histogram():
m_nBins(0), m_entries(0), m_binCentres(NULL), m_binValues(NULL)
{
}

Histogram::Histogram(std::string name, std::string particle, std::string type, 
					 double time, double minBin, double maxBin, unsigned int nBins)
{
	Initialise(name, particle, type, time, minBin, maxBin, nBins);
}

Histogram::~Histogram()
{
	delete [] m_binCentres;
	delete [] m_binValues;
}

void Histogram::Initialise(std::string name, std::string particle , std::string type, 
						   double time, double minBin, double maxBin, unsigned int nBins)
{
	m_name = name;
	m_particle = particle;
	m_type = type;
	m_time = time;
	m_nBins = nBins;
	m_binCentres = new double [nBins];
	m_binValues  = new double [m_nBins];
	double detla = (maxBin - minBin) / (m_nBins - 1.0);
	for (unsigned int i = 0; i < m_nBins; i++)
	{
		m_binCentres[i] = minBin + i * detla;
		m_binValues[i] = 0;
	}
}

void Histogram::AppParticle(Particle* part)
{
	if (m_nBins == 0)
	{
		std::cerr << "Error: Histogram has not been initialise!\n";
		exit(1);
	} else
	{
		if (m_type == "Energy" || "energy")
		{
			m_entries++;
			double energy = part->GetEnergy();
			if (energy > m_binCentres[0] && energy < m_binCentres[m_nBins-1])
			{
				unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, energy);
				m_binValues[index] += part->GetWeight();
			}
		} else if (m_type == "X" || "x")
		{
			m_entries++;
			double xPos = part->GetPosition()[0];
			if (xPos > m_binCentres[0] && xPos < m_binCentres[m_nBins-1])
			{
				unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, xPos);
				m_binValues[index] += part->GetWeight();
			}
		} else if (m_type == "Y" || "y")
		{
			m_entries++;
			double yPos = part->GetPosition()[1];
			if (yPos > m_binCentres[0] && yPos < m_binCentres[m_nBins-1])
			{
				unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, yPos);
				m_binValues[index] += part->GetWeight();
			}
		} else if (m_type == "Z" || "z")
		{
			m_entries++;
			double zPos =  part->GetPosition()[2];
			if (zPos > m_binCentres[0] && zPos < m_binCentres[m_nBins-1])
			{
				unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, zPos);
				m_binValues[index] += part->GetWeight();
			}
		} else if (m_type == "PX" || "px")
		{
			m_entries++;
			double xPos = part->GetMomentum()[0];
			if (xPos > m_binCentres[0] && xPos < m_binCentres[m_nBins-1])
			{
				unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, xPos);
				m_binValues[index] += part->GetWeight();
			}
		} else if (m_type == "PY" || "py")
		{
			m_entries++;
			double yPos = part->GetMomentum()[1];
			if (yPos > m_binCentres[0] && yPos < m_binCentres[m_nBins-1])
			{
				unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, yPos);
				m_binValues[index] += part->GetWeight();
			}
		} else if (m_type == "PZ" || "pz")
		{
			m_entries++;
			double zPos =  part->GetMomentum()[2];
			if (zPos > m_binCentres[0] && zPos < m_binCentres[m_nBins-1])
			{
				unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, zPos);
				m_binValues[index] += part->GetWeight();
			}		
		} else
		{
			std::cerr << "Error: Particle property \"" << m_type << "\" not found\n";
			exit(1);
		}
	}
}

void Histogram::Fill(ParticleList* partList)
{
	if (m_nBins == 0)
	{
		std::cerr << "Error: Histogram has not been initialise!\n";
		exit(1);
	} else
	{
		if (m_type == "Energy" || "energy")
		{
			for (unsigned int i = 0; i < partList->GetNPart(); i++)
			{
				m_entries++;
				double energy =  partList->GetParticle(i)->GetEnergy();
				if (energy > m_binCentres[0] && energy < m_binCentres[m_nBins-1])
				{
					unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, energy);
					m_binValues[index] += partList->GetParticle(i)->GetWeight();
				}
			}
			
		} else if (m_type == "X" || "x")
		{
			for (unsigned int i = 0; i < partList->GetNPart(); i++)
			{
				m_entries++;
				double xPos =  partList->GetParticle(i)->GetPosition()[0];
				if (xPos > m_binCentres[0] && xPos < m_binCentres[m_nBins-1])
				{
					unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, xPos);
					m_binValues[index] += partList->GetParticle(i)->GetWeight();
				}
			}
		} else if (m_type == "Y" || "y")
		{
			for (unsigned int i = 0; i < partList->GetNPart(); i++)
			{
				m_entries++;
				double yPos =  partList->GetParticle(i)->GetPosition()[1];
				if (yPos > m_binCentres[0] && yPos < m_binCentres[m_nBins-1])
				{
					unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, yPos);
					m_binValues[index] += partList->GetParticle(i)->GetWeight();
				}
			}
		} else if (m_type == "Z" || "z")
		{
			for (unsigned int i = 0; i < partList->GetNPart(); i++)
			{
				m_entries++;
				double zPos =  partList->GetParticle(i)->GetPosition()[2];
				if (zPos > m_binCentres[0] && zPos < m_binCentres[m_nBins-1])
				{
					unsigned int index = Numerics::ArrayIndex(m_binCentres, m_nBins, zPos);
					m_binValues[index] += partList->GetParticle(i)->GetWeight();
				}
			}
		} else
		{
			std::cerr << "Error: Particle property \"" << m_type << "\" not found\n";
			exit(1);
		}
	}
	Normalise();
}

void Histogram::Merge(Histogram* hist)
{ 
	// Check that hisograms are compitble
	if (m_nBins == hist->m_nBins && m_binCentres[0] == hist->m_binCentres[0] &&
		m_binCentres[m_nBins-1] == hist->m_binCentres[m_nBins-1])
	{
		m_entries += hist->m_entries;
		for (unsigned int i = 0; i < m_nBins; i++)
		{
			m_binValues[i] += hist->m_binValues[i];
		}
	} else
	{
		std::cerr << "Error: Trying to merge histograms \"" << m_name << "\" and \""
				  << hist->m_name << "\".\n";
		std::cerr << "These histograms are incompatible.\n";
	}
	delete hist;
}

void Histogram::Normalise()
{
	if (m_entries > 0)
	{
		double sum(0);
		for (unsigned int i = 0; i < m_nBins; i++)
		{
			sum += m_binValues[i];
		}
		for (unsigned int i = 0; i < m_nBins; i++)
		{
			m_binValues[i] /= sum;
		}
	}
}