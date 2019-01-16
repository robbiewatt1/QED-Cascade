#ifndef OUTPUTMANAGER_HH
#define OUTPUTMANAGER_HH

#include <vector>
#include <string>
#include "HDF5Output.hh"
#include "Particle.hh"
#include "ParticleList.hh"
#include "Field.hh"
#include "UnitsSystem.hh"

class OutputManager
{
public:
	// Output using normilised units. Fails for QED processes
	OutputManager(HDF5Output* file);

	// Output using units defined by units class
	OutputManager(HDF5Output* file, UnitsSystem* units);

	~OutputManager();

	void SetFields(bool partOn = false, bool fieldOn = false, bool qedOn = false);

	// Particle Output methods
	void SingleParticle(const Particle& part, std::string name);

	void ListParticle(ParticleList* partList, std::string name);

	// Physics package output methods
	void EMField(Field* field, const std::vector<double> &tAxis,
							   const std::vector<double> &xAxis,
							   const std::vector<double> &yAxis,
							   const std::vector<double> &zAxis);

private:
	HDF5Output* m_outputFile;
	UnitsSystem* m_units;

	// Boolians to avoid adding the same group twice
	bool m_singlePartBool = false;
	bool m_partListBool = false;
};
#endif