#ifndef HDF5OUTPUT_HH
#define HDF5OUTPUT_HH

#include "H5Cpp.h"

class HDF5Output
{
public:
	HDF5Output();

	HDF5Output(std::string fileName, bool append = false);
	
	~HDF5Output();

	// Adds 1D vector data structure to the  
	void AddVector(double* data, hsize_t length, std::string name);

	// Adds 2D matrix data srtucture
	void AddMatrix(double* data, hsize_t xLength, hsize_t yLength, std::string name);

private:
	H5::H5File* m_file;
	std::string m_fileName;
};

#endif