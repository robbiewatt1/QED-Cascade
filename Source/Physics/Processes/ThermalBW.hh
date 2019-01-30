#ifndef ThermalBW_HH
#define ThermalBW_HH

#include "Process.hh"
#include "ThermalRadField.hh"

class ThermalBW: public Process
{
public:
	ThermalBW(ThermalRadField* field, double dt, double maxEnergy = 2000, 
			 std::string table = "./ThermalBW.table", bool saveTable = false);

	~ThermalBW();

	void UseTable(std::string table);

	void Interact(Particle &part);
	
private:

	double CrossSection(double s);

	double DiffCrossSection(double s, double theta);

	void LoadTables(std::string table);

	void CalculateTables();

	void SaveTable();

private:
	ThermalRadField* m_field;
	double m_dt;
	double m_maxEnergy;
	bool m_saveTable;


	// data for optical depth table
	unsigned int m_energySize;
	unsigned int m_tempSize;
	double* m_energy;
	double* m_temperature;
	double** m_opticalDepth;

	unsigned int m_intSize = 100;	// resolusion of integrals

};
#endif