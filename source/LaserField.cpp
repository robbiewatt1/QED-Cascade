#include "LaserField.hh"

LaserField::LaserField()
{
	m_Efield = NULL;
	m_Bfield = NULL;
}

LaserField::LaserField(std::vector<unsigned int> dims, std::vector<double> max):
m_dims(dims), m_max(max)
{
	AllocateField();
}

LaserField::~LaserField()
{
	if (m_Efield != NULL || m_Bfield != NULL)
	{
		DeallocateField();
	}
}

void LaserField::AllocateField()
{
	m_xAxis = std::vector<double>(m_dims[0]);
	for (unsigned int i = 0; i < m_dims[0]; i++)
	{
		m_xAxis[i] = (double)i * m_max[0] / (double)m_dims[0];
	}
	m_yAxis = std::vector<double>(m_dims[1]);
	for (unsigned int i = 0; i < m_dims[1]; i++)
	{
		m_yAxis[i] = (double)i * m_max[1] / (double)m_dims[1];
	}
	m_zAxis = std::vector<double>(m_dims[2]);
	for (unsigned int i = 0; i < m_dims[2]; i++)
	{
		m_yAxis[i] = (double)i * m_max[2] / (double)m_dims[2];
	}

	m_Efield = new double*** [3];
	m_Bfield = new double*** [3];
	for (unsigned int dir = 0; dir < 3; dir++)
	{
		m_Efield[dir] = new double** [m_dims[0]];
		m_Bfield[dir] = new double** [m_dims[0]];
		for (unsigned int i = 0; i < m_dims[0]; i++)
		{
			m_Efield[dir][i] = new double* [m_dims[1]];
			m_Bfield[dir][i] = new double* [m_dims[1]];
			for (unsigned int j = 0; j < m_dims[1]; j++)
			{
				m_Efield[dir][i][j] = new double [m_dims[2]];
				m_Bfield[dir][i][j] = new double [m_dims[2]];
				for (unsigned int k = 0; k < m_dims[2]; k++)
				{
					m_Efield[dir][i][j][k] = 0.0;
					m_Bfield[dir][i][j][k] = 0.0;
				}
			}
		}
	}
}

void LaserField::DeallocateField()
{
	for (unsigned int dir = 0; dir < 3; dir++)
	{
		for (unsigned int i = 0; i < m_dims[0]; i++)
		{

			for (unsigned int j = 0; j < m_dims[1]; j++)
			{
				delete [] m_Efield[dir][i][j];
				delete [] m_Bfield[dir][i][j];
			}
			delete [] m_Efield[dir][i];
			delete [] m_Bfield[dir][i];
		}
		delete [] m_Efield[dir];
		delete [] m_Bfield[dir];
	}
	delete [] m_Efield;
	delete [] m_Bfield;
	m_Efield = NULL;
	m_Bfield = NULL;
}


double LaserField::GetEfield(unsigned int dir, unsigned int i, unsigned int j, unsigned int k) const
{
	return m_Efield[dir][i][j][k];
}

double LaserField::GetBfield(unsigned int dir, unsigned int i, unsigned int j, unsigned int k) const
{
	return m_Bfield[dir][i][j][k];
}