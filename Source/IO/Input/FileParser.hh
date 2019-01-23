#ifndef FileParser_HH
#define FILEPARSER_HH

#include "INIReader.hh"
#include "UnitsSystem.hh"

struct GeneralParameters
{
	// General
	double timeStep;	// time step
	double timeEnd;		// end of simulation
};

struct ParticleParameters
{
	long unsigned int maxParticle;	// max number of particles per list
	unsigned int numbSources; 		// number of particle sources
	std::string sourceNames;		// names of sources
	std::string sourceType;			// particle types
	ThreeVector sourcePosition;		// positions
	ThreeVector sourceDirection;	// directions
	double sourceEnergy;			// source energy
	double sourceSize;				// source size
};

struct FieldParameters
{
	std::string fieldType;	// type of field
	// for static
	ThreeVector fieldE;	// electric field
	ThreeVector fieldB;	// magnetic field 
	// for guassian or plane
	double fieldMaxE;			// Max E field
	double fieldWavelength;		// laser wavelength
	double fieldDuration;		// laser duration
	double fieldWaist;			// laser waist
	double fieldPolerisation;	// laser polerisation
	ThreeVector fieldStart;		// start point of laser pulse
	ThreeVector fieldFocus;		// focus point of laser
};

struct HistogramParameters
{
	unsigned int numbHist;	// number of histograms
	std::string histName;	// names of the histograms
	std::string particle;	// particles being histed
	double histTime;		// Time at which hists are made
	unsigned int bins;		// number of bins in histograms
	double minBin;			// First value of bins 
	double maxBin;			// last value of bins
};

class FileParser
{
public:
	FileParser(std::string fileName);

	~FileParser();

	GeneralParameters GetGeneral() const;

	FieldParameters GetField() const;

	std::vector<ParticleParameters> GetParticle() const;

	std::vector<HistogramParameters> GetHistograms() const;

private:

	// Checks that the vital information needed for the simulation 
	// has been set
	void CheckVitals();

private:
	std::vector<std::string> m_sections;
	GeneralParameters m_general;
	FieldParameters m_fields;
	std::vector<ParticleParameters> m_particles;
	std::vector<HistogramParameters> m_histograms;

	INIReader* m_reader;
};

#endif