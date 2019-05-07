#ifndef OUTPUTMANAGER_HH
#define OUTPUTMANAGER_HH

#include <vector>
#include <string>
#include "ThreeVector.hh"
#include "HDF5Output.hh"
#include "Particle.hh"
#include "ParticleList.hh"
#include "EMField.hh"
#include "NonLinearCompton.hh"
#include "UnitsSystem.hh"
#include "Histogram.hh"

class OutputManager
{
public:
	// Output using normilised units. Fails for QED processes
	OutputManager(std::string fileName);

	// Output using units defined by units class
	OutputManager(std::string fileName, UnitsSystem* units);

	~OutputManager();

	// Particle Output methods
	void SingleParticle(Particle* part, std::string name);

	// Saves the data from a tracked particle
	void ListTracks(ParticleList* partList, std::string name);

	// Saves data for a non tracked particle list
	// format: id, energy, theta1, theta2, pos1, pos2, pos3,  
	void ListProperties(ParticleList* partList, std::string setName);

	// Outputs the full data of a particle source before and after the interaction
	// this data is then used to train NN for the given interaction
	void InitSource(unsigned int nEvents);

	void StoreSource(ParticleList* partList, unsigned int eventID, bool primary);

	void StoreTrack(ParticleList* partList, unsigned int eventID);

	void OutputEvents(bool outSource, bool outTrack);

	// Physics package output methods
	void OutEMField(EMField* field, const std::vector<double> &tAxis,
							    	const std::vector<double> &xAxis,
							    	const std::vector<double> &yAxis,
							    	const std::vector<double> &zAxis);

	void OutputHist(Histogram* hist);

private:
	HDF5Output* m_outputFile;
	UnitsSystem* m_units;

	// Boolians to avoid adding the same group twice
	bool m_singlePartBool = false;
	bool m_partListBool = false;
	bool m_particleSourceBool = false;

	// Structures holding data for source outputs
	std::vector<double*> m_primaryEvent;
	std::vector<double*> m_electronEvent;
	std::vector<double*> m_positronEvent;
	std::vector<double*> m_photonEvent;
	std::vector<unsigned int> m_electronCount;
	std::vector<unsigned int> m_positronCount;
	std::vector<unsigned int> m_photonCount;

	// structures holding data for tracking output
	std::vector<unsigned int> m_idTrack;
	std::vector<std::vector<ThreeVector>> m_positionTrack;
	std::vector<std::vector<ThreeVector>> m_momentumTrack;
	std::vector<std::vector<double>> m_timeTrack;
	std::vector<std::vector<double>> m_gammaTrack;
};
#endif