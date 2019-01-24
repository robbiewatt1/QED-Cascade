#ifndef HISTOGRAM_HH
#define HISTOGRAM_HH

#include <string>
#include "ParticleList.hh"

class Histogram
{
public:
	Histogram();

	Histogram(std::string name, std::string particle, std::string type, 
			  double time, double minBin, double maxBin, unsigned int nBins);

	~Histogram();

	void Initialise(std::string name, std::string particle, std::string type, double time,
					double minBin, double maxBin, unsigned int nBins);

	void Fill(ParticleList* partList);

	void Merge(Histogram* hist);

	std::string GetName() const {return m_name;}

	std::string GetParticle() const {return m_particle;}

	double GetTime() const {return m_time;}

	double* GetBinCentres() const {return m_binCentres;}

	double* GetBinValues() const {return m_binValues;}

	unsigned int GetNBins() const {return m_nBins;}

private:
	void Normalise();

private:
	std::string m_name;
	std::string m_type;
	std::string m_particle;
	unsigned int m_nBins;
	unsigned int m_entries;
	double m_time;
	double* m_binCentres;
	double* m_binValues;
};
#endif