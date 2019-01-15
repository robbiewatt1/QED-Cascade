#ifndef THREEVECTOR_HH
#define THREEVECTOR_HH

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

#include "ThreeMatrix.hh"

class ThreeVector
{
private:
	double m_data[3];

public:
	// Defult Constructor
	ThreeVector()
	{
		m_data[0] = 0.0;
		m_data[1] = 0.0;
		m_data[2] = 0.0;
	}

	// Copy constructor
	ThreeVector(const ThreeVector &vector)
	{
		m_data[0] = vector.m_data[0];
		m_data[1] = vector.m_data[1];
		m_data[2] = vector.m_data[2];
	}

	ThreeVector(double x, double y, double z)
	{
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
	}

	~ThreeVector()
	{
	}

	// Prints the vector to the screan
	void Print() const
	{
		std::cout << "[" << m_data[0] << ", " << m_data[1] << ", " << m_data[2] << "]" << std::endl;
	}

	// Returns the cross product
	ThreeVector Cross(const ThreeVector& vector) const
	{
		ThreeVector newVector;
		newVector.m_data[0] = m_data[1] * vector.m_data[2] - m_data[2] * vector.m_data[1];
		newVector.m_data[1] = m_data[2] * vector.m_data[0] - m_data[0] * vector.m_data[2];
		newVector.m_data[2] = m_data[0] * vector.m_data[1] - m_data[1] * vector.m_data[0];
		return newVector;
	}

	ThreeVector Norm() const
	{
		return *this / Mag();
	}

	// Returns the dot product
	double Dot(const ThreeVector& vector) const
	{
		return m_data[0] * vector.m_data[0] + m_data[1] * vector.m_data[1] 
											+ m_data[2] * vector.m_data[2];
	}

	// Returns the magnitude
	double Mag() const
	{
		return std::sqrt(Dot(*this));
	}

	// Returns the magnitude squared
	double Mag2() const
	{
		return Dot(*this);
	}

	// Returns the matrix required to rotate the vecotor onto the second
	ThreeMatrix RotateToAxis(const ThreeVector &axis) const
	{
		ThreeMatrix rotation;
		// Fist we need to check if the vectors are ainti parallel as this method fails
		if ((this->Norm() + axis.Norm()).Mag2() <= 1e-8)
		{
			rotation[0][0] = -1.0;
			rotation[1][1] = -1.0;
			rotation[2][2] = -1.0;
		} else
		{
			ThreeVector norm1 = this->Norm();
			ThreeVector norm2 = axis.Norm();
			ThreeVector crossVec = norm1.Cross(norm2);

			double cos = norm1.Dot(norm2);
			ThreeMatrix skewMat;
			skewMat[0][1] = -crossVec[2];
			skewMat[0][2] =  crossVec[1];
			skewMat[1][0] =  crossVec[2];
			skewMat[1][2] = -crossVec[0];
			skewMat[2][0] = -crossVec[1];
			skewMat[2][1] =  crossVec[0];

			ThreeMatrix ident;
			ident.Ideneity();
			rotation = ident + skewMat + skewMat * skewMat * (1.0 / (1.0 + cos));
		}
		return rotation;
	}

public:
	// returns the value of the vector at elementIndex. This method allows you to edit the vector data.
	double& operator[](unsigned int index)
	{
		assert(index >= 0 && index < 3);
		return m_data[index];
	}

	double operator[](unsigned int index) const
	{
		assert(index >= 0 && index < 3);
		return m_data[index];
	}

	// Copy vecotor to new vector
	ThreeVector& operator=(const ThreeVector &vector)
	{

		// Self assigment gaurd
		if (this == &vector)
		{
			return *this;
		}
		m_data[0] = vector.m_data[0];
		m_data[1] = vector.m_data[1];
		m_data[2] = vector.m_data[2];

		return *this;
	}

	//vector-vector operations:
	friend ThreeVector operator+(const ThreeVector &vector1, const ThreeVector &vector2)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector1.m_data[0] + vector2.m_data[0];
		newVector.m_data[1] = vector1.m_data[1] + vector2.m_data[1];
		newVector.m_data[2] = vector1.m_data[2] + vector2.m_data[2];
		return newVector;
	};

	friend ThreeVector operator-(const ThreeVector &vector1, const ThreeVector &vector2)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector1.m_data[0] - vector2.m_data[0];
		newVector.m_data[1] = vector1.m_data[1] - vector2.m_data[1];
		newVector.m_data[2] = vector1.m_data[2] - vector2.m_data[2];
		return newVector;
	};

	friend ThreeVector operator*(const ThreeVector &vector1, const ThreeVector &vector2)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector1.m_data[0] * vector2.m_data[0];
		newVector.m_data[1] = vector1.m_data[1] * vector2.m_data[1];
		newVector.m_data[2] = vector1.m_data[2] * vector2.m_data[2];
		return newVector;
	};

	friend ThreeVector operator/(const ThreeVector &vector1, const ThreeVector &vector2)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector1.m_data[0] / vector2.m_data[0];
		newVector.m_data[1] = vector1.m_data[1] / vector2.m_data[1];
		newVector.m_data[2] = vector1.m_data[2] / vector2.m_data[2];
		return newVector;
	};

	//T-vector operations:
	friend ThreeVector operator+(double scalar, const ThreeVector &vector)
	{
		ThreeVector newVector;
		newVector.m_data[0] = scalar + vector.m_data[0];
		newVector.m_data[1] = scalar + vector.m_data[1];
		newVector.m_data[2] = scalar + vector.m_data[2];
		return newVector;
	};

	friend ThreeVector operator+(const ThreeVector &vector, double scalar)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector.m_data[0] + scalar;
		newVector.m_data[1] = vector.m_data[1] + scalar;
		newVector.m_data[2] = vector.m_data[2] + scalar;
		return newVector;
	};

	friend ThreeVector operator-(double scalar, const ThreeVector &vector)
	{
		ThreeVector newVector;
		newVector.m_data[0] = scalar - vector.m_data[0];
		newVector.m_data[1] = scalar - vector.m_data[1];
		newVector.m_data[2] = scalar - vector.m_data[2];
		return newVector;
	};

	friend ThreeVector operator-(const ThreeVector &vector, double scalar)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector.m_data[0] - scalar;
		newVector.m_data[1] = vector.m_data[1] - scalar;
		newVector.m_data[2] = vector.m_data[2] - scalar;
		return newVector;
	};

	friend ThreeVector operator*(double scalar, const ThreeVector &vector)
	{
		ThreeVector newVector;
		newVector.m_data[0] = scalar * vector.m_data[0];
		newVector.m_data[1] = scalar * vector.m_data[1];
		newVector.m_data[2] = scalar * vector.m_data[2];
		return newVector;
	};

	friend ThreeVector operator*(const ThreeVector &vector, double scalar)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector.m_data[0] * scalar;
		newVector.m_data[1] = vector.m_data[1] * scalar;
		newVector.m_data[2] = vector.m_data[2] * scalar;
		return newVector;
	};

	friend ThreeVector operator/(double scalar, const ThreeVector &vector)
	{
		ThreeVector newVector;
		newVector.m_data[0] = scalar / vector.m_data[0];
		newVector.m_data[1] = scalar / vector.m_data[1];
		newVector.m_data[2] = scalar / vector.m_data[2];
		return newVector;
	};

	friend ThreeVector operator/(const ThreeVector &vector, double scalar)
	{
		ThreeVector newVector;
		newVector.m_data[0] = vector.m_data[0] / scalar;
		newVector.m_data[1] = vector.m_data[1] / scalar;
		newVector.m_data[2] = vector.m_data[2] / scalar;
		return newVector;
	};

	//Matrix-Vector operations
	friend ThreeVector operator*(const ThreeMatrix &matrix, const ThreeVector &vector)
	{
		ThreeVector newVector;
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				newVector[i] += matrix[i][j] * vector[j];

			}
		}
		return newVector;
	}
};
#endif