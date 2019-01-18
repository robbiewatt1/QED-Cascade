#ifndef HISTOGRAM_HH
#define HISTOGRAM_HH

#include <string>
#include "ParticleList.hh"

class Histogram
{
public:
	Histogram();

	~Histogram();

	void Initialise(std::string name, double minBin, double maxBin, unsigned int nBins);

	void Fill(ParticleList* partList, string dataType);

	void Merge(Histogram* hist);

private:
	void Normalise();

private:
	unsigned int m_nBins;
	unsigned int m_entries
	double* m_binCentres;
	double* m_binValues;

	std::string m_name;
};
#endif