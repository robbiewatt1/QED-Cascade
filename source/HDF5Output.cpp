#include <iostream>

#include "HDF5Output.hh"

HDF5Output::HDF5Output():
m_file(NULL)
{
}

HDF5Output::HDF5Output(std::string fileName, bool append):
m_fileName(fileName) 
{
	if (append == true)
	{
		H5::H5File* m_file = new H5::H5File(m_fileName, H5F_ACC_RDWR);
	} else
	{
		H5::H5File* m_file = new H5::H5File(m_fileName, H5F_ACC_TRUNC);
	}
}

HDF5Output::~HDF5Output()
{
	delete m_file;
}

void HDF5Output::AddVector(double* data, hsize_t length, std::string dataName)
{
	H5::DataSet* set = new H5::DataSet(m_file->createDataSet(dataName, H5::PredType::NATIVE_DOUBLE,
														   	  H5::DataSpace(1, &length)));
	set->write(data, H5::PredType::NATIVE_DOUBLE);
	delete set;
}

void HDF5Output::AddMatrix(double* data, hsize_t xLength, hsize_t yLength,
						   std::string dataName)
{
	hsize_t dimensions[2];// = {xLength, yLength};
	dimensions[0] = xLength;
	dimensions[1] = yLength;
	std::cerr << xLength << yLength << std::endl;

	std::cerr << "failed" << std::endl;

	H5::DataSet* set = new H5::DataSet(m_file->createDataSet(dataName, H5::PredType::NATIVE_DOUBLE,
								  						 	   H5::DataSpace(2, dimensions)));
	set->write(data, H5::PredType::NATIVE_DOUBLE);
	delete set;
}