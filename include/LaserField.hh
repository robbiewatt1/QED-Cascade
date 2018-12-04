#include <vector>
#include "H5Cpp.h"

class LaserField
{
public:

	LaserField();

	LaserField(std::vector<unsigned int> dims, std::vector<double> max);
	
	~LaserField();

	void AllocateField();

	void DeallocateField();

	void InitField();	// Test function used to initilise the field

	void InitFieldHDF5(std::string fileName);	// Initilises the field using a HDF5 data file

	double GetEfield(unsigned int dir, unsigned int i, unsigned int j, unsigned int k) const;

	double GetBfield(unsigned int dir, unsigned int i, unsigned int j, unsigned int k) const;


private:
	double**** m_Efield;	// array containing Electric field information, first index gives direction
	double**** m_Bfield;	// array containing Magnetic field information, first index gives direction
	std::vector<unsigned int> m_dims;	// array of size of dimsensions
	std::vector<double> m_max;	// array of maximum spacial value of field
	std::vector<double> m_xAxis;
	std::vector<double> m_yAxis;
	std::vector<double> m_zAxis;
};