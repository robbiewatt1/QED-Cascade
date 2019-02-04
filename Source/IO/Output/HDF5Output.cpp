#include <iostream>

#include "HDF5Output.hh"

HDF5Output::HDF5Output():
m_file(NULL), m_group(NULL)
{
}

HDF5Output::HDF5Output(std::string fileName, bool append):
m_fileName(fileName), m_group(NULL)
{
	if (append == true)
	{
		m_file = new H5::H5File(m_fileName, H5F_ACC_RDWR);
	} else
	{
		m_file = new H5::H5File(m_fileName, H5F_ACC_TRUNC);
	}
}

HDF5Output::~HDF5Output()
{
	delete m_group;
	delete m_file;
}

void HDF5Output::AddGroup(std::string groupName)
{
	if (m_group != NULL)
	{
		delete m_group;
	}
	bool status = H5Gget_objinfo(m_file->getId(), groupName.c_str(), 0, NULL);
	if (status != 0)
	{
		m_group = new H5::Group(m_file->createGroup(groupName));
	}
}

void HDF5Output::AddSubGroup(std::string subGroupName)
{
	if (m_subGroup != NULL)
	{
		delete m_subGroup;
	}
	m_subGroup = new H5::Group(m_file->createGroup(subGroupName));
}

void HDF5Output::AddArray1D(double* data, hsize_t length, std::string dataName)
{
	H5::DataSet* set = new H5::DataSet(m_file->createDataSet(dataName, H5::PredType::NATIVE_DOUBLE,
														   	  H5::DataSpace(1, &length)));
	set->write(data, H5::PredType::NATIVE_DOUBLE);
	delete set;
}

void HDF5Output::AddArray2D(double* data, hsize_t xLength, hsize_t yLength,
						   std::string dataName)
{
	hsize_t dimensions[2] = {xLength, yLength};

	H5::DataSet* set = new H5::DataSet(m_file->createDataSet(dataName, H5::PredType::NATIVE_DOUBLE,
								  						 	   H5::DataSpace(2, dimensions)));
	set->write(data, H5::PredType::NATIVE_DOUBLE);
	delete set;
}

void HDF5Output::AddArray3D(double* data, hsize_t xLength, hsize_t yLength, hsize_t zLength, 
						   std::string dataName)
{
	hsize_t dimensions[3] = {xLength, yLength, zLength};

	H5::DataSet* set = new H5::DataSet(m_file->createDataSet(dataName, H5::PredType::NATIVE_DOUBLE,
								  						 	   H5::DataSpace(3, dimensions)));
	set->write(data, H5::PredType::NATIVE_DOUBLE);
	delete set;
}