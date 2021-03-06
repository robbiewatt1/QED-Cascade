#ifndef HDF5OUTPUT_HH
#define HDF5OUTPUT_HH

#include "H5Cpp.h"

class HDF5Output
{
public:
	HDF5Output();

	HDF5Output(std::string fileName, bool append = false);
	
	~HDF5Output();

	void AddGroup(std::string groupName);

	void AddSubGroup(std::string subGroup);
	
	// Adds 1D vector data structure to the  
	void AddArray1D(double* data, hsize_t length, std::string name);

	// Adds 2D matrix data srtucture
	void AddArray2D(double* data, hsize_t xLength, hsize_t yLength, std::string name);

	// Adds 3D matrix data structure
	void AddArray3D(double* data, hsize_t xLength, hsize_t yLength, hsize_t zLength, 
					std::string dataName);

private:
	std::string m_fileName;
	H5::H5File* m_file;
	H5::Group* m_group;
	H5::Group* m_subGroup;
};

#endif