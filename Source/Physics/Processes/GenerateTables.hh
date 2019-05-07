#ifndef GENERATETABLES_HH
#define GENERATETABLES_HH

#include <string>

class GenerateTables
{
public:
	GenerateTables(std::string fileName);

	~GenerateTables();

	void RadReactTable(double chiMin, double chiMax, double etaMin, double etaMax,
			unsigned int resolusion);

	void BreitWheelerTable();

private:

	
};
#endif