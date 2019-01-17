#include "Histogram.hh"

Histogram::Histogram():
m_binCentres(NULL), m_entries(NULL)
{
}

Histogram::Histogram(std::string name, double minBin, double maxBin, unsigned int nBins):
{
	Initialise(name, minBin, maxBin, nBins);
}

Histogram::~Histogram()
{
	delete [] m_binCentres;
	delete [] m_binValues;
}

void Histogram::Initialise(std::string name, double minBin, double maxBin, unsigned int nBins)
{
	m_nane = name;
	m_nBins = nBins;
	m_binCentres = new double [nBins];
	m_binValues  = new double [m_nBins]
	double detla = (maxBin - minBin) / (m_nBins - 1.0);
	for (unsigned int i = 0; i < m_nBins; i++)
	{
		m_binCentres[i] = minBin + i * detla;
		m_binValues[i] = 0;
	}
}

void Histogram::Merge(Histogram* hist)
{
	// Check that hisograms are compitble
	if (m_nBins == hist.m_nBins && m_binCentres[0] == hist.m_binCentres[0] &&
		m_binCentres[m_nBins-1] == hist.m_binCentres[m_nBins-1])
	{
		m_entries += hist.m_entries;
		for (unsigned int i = 0; i < m_nBins; i++)
		{
			m_binValues[i] += hist.m_binValues[i];
		}
	} else
	{
		std::cerr << "Error: Trying to merge histograms \"" << m_name << "\" and \""
				  << hist.m_name << "\".\n";
		std::cerr << "These histograms are incompatible.\n";
	}
	delete hist;
}