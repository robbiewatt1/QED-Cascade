#ifndef LINEARBW_HH
#define LINEARBW_HH

#include "Process.hh"
#include "RadField.HH"

class LinearBW: public Process
{
public:
	LinearBW(RadField* field, double dt, std::string table = "./linearBW.table",
			 bool saveTable = false);

	~LinearBW();

	void UseTable(std::string table);

	void Interact(Particle &part);
	
private:

	double CrossSection(double s);

	double DiffCrossSection(double s, double theta);

	void LoadTables();

	void CalculateTables();

private:
	RadField* m_field;
	double m_dt;
	bool m_saveTable;

	// data for optical depth table
	unsigned int m_od_length;
	double* m_od_energyAxis;
	double* m_od_dataTable;

	// data for photon energy sampling
	unsigned int m_phEn_length;
	double* m_phEn_gammaAxis;
	double* m_phEn_xRayAxis;
	double** m_dataTable;
};
#endif