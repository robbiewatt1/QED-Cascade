#ifndef VECTOR_HH
#define VECTOR_HH

#include <iostream>
#include <cassert>
#include <iomanip>

template<typename T>
class Vector
{
private:
	int m_nElements;
	T *m_data;

public:
	// Defult Constructor
	Vector():
		m_nElements(0), m_data(nullptr)
	{
	}

	// Copy constructor
	Vector(const Vector &vector)
	{
		m_nElements = vector.m_nElements;
		m_data = new T [vector.m_nElements];
		for (int i = 0; i < m_nElements; i++)
		{
			m_data[i] = vector.m_data[i];
		}
	}

	// Constructs an empty vector of nElement length
	Vector(int nElements):
		m_nElements(nElements)
	{
		// Allocate the vecotr memenory
		m_data = new T [m_nElements];

		// Check if T is double / int /complex. If so then set all values to 0;
		for (int i = 0; i < m_nElements; i++)
		{
	//		m_data[i] = 0;
		}
	}

	// Defult Destructor. Deletes the data to avoid memory leaks
	~Vector()
	{
		delete [] m_data;
	}

	// Return the length of the Vector
	int Length() const
	{
		return m_nElements;
	}

	// Return the first element of the array
	T Start() const
	{
		return m_data[0];
	}

	// Return the last element of the array
	T End() const
	{
		return m_data[m_nElements - 1];
	}

	void Append(T newElement)
	{
		T *dataNew = new T [m_nElements + 1];

		for (int i = 0; i < m_nElements; i++)
		{
			dataNew[i] = m_data[i];
		}
		dataNew[m_nElements] = newElement;

		delete[] m_data;
		m_data = dataNew;
		++m_nElements;
	}

	// Prints the vector to the screan
	void Print() const
	{
		if(m_nElements == 1)
		{
			std::cout << "[ " << std::fixed << std::setprecision(5) << m_data[0] << "]" << std::endl;;
		} else
		{
			std::cout << "[ " << std::fixed << std::setprecision(5) << m_data[0] << "\n";
			for (int i = 1; i < m_nElements - 1; i++)
			{
				std::cout << "  " << std::fixed << std::setprecision(5) << m_data[i] << "\n";
			}
			std::cout << "  " << std::fixed << std::setprecision(5) << m_data[m_nElements - 1]
			          << " ]" << std::endl;
		}
	}

public:

	// returns the value of the vector at elementIndex. This method does not allow you to change the value of vector data.
	// This should be used when passing Vector to class as const.
	T& operator()(const int elementIndex) const
	{
		assert(elementIndex >= 0 && elementIndex < m_nElements);
		return m_data[elementIndex];
	}

	// returns the value of the vector at elementIndex. This method allows you to edit the vector data.
	T& operator[](const int elementIndex)
	{
		assert(elementIndex >= 0 && elementIndex < m_nElements);
		return m_data[elementIndex];
	}

	// returns the value of the vector at elementIndex. This method does not allow data to be editted
	T& operator[](const int elementIndex) const
	{
		assert(elementIndex >= 0 && elementIndex < m_nElements);
		return m_data[elementIndex];
	}

	// Copy vecotor to new vector
	Vector<T>& operator=(const Vector<T> &vector)
	{

		// Self assigment gaurd
		if (this == &vector)
		{
			return *this;
		}

		// Delete data held by new vector
		delete[] m_data;

		// Copy variables over
		m_nElements = vector.m_nElements;
		if (vector.m_data)
		{
			// Allcoate new array space
			m_data = new T [m_nElements];
			// Copy data
			for (int i = 0; i < m_nElements; i++)
			{
				m_data[i] = vector.m_data[i];
			}
		} else
		{
			m_data = 0;
		}

		return *this;
	}

	//vector-vector operations:
	friend Vector<T> operator+(const Vector<T> &vector1, const Vector<T> &vector2)
	{
		assert(vector1.m_nElements == vector2.m_nElements);
		Vector<T> newVector = Vector<T>(vector1.m_nElements);
		for (int i = 0; i < vector1.m_nElements; i++)
		{
			newVector(i) = vector1.m_data[i] + vector2.m_data[i];	
		}
		return newVector;
	};

	friend Vector<T> operator-(const Vector<T> &vector1, const Vector<T> &vector2)
	{
		assert(vector1.m_nElements == vector2.m_nElements);
		Vector<T> newVector = Vector<T>(vector1.m_nElements);
		
		for (int i = 0; i < vector1.m_nElements; i++)
		{
			newVector(i) = vector1.m_data[i] - vector2.m_data[i];
		}
		return newVector;
	};

	friend Vector<T> operator*(const Vector<T> &vector1, const Vector<T> &vector2)
	{
		assert(vector1.m_nElements == vector2.m_nElements);
		Vector<T> newVector = Vector<T>(vector1.m_nElements);
		
		for (int i = 0; i < vector1.m_nElements; i++)
		{
			newVector(i) = vector1.m_data[i] * vector2.m_data[i];
		}
		return newVector;
	};

	friend Vector<T> operator/(const Vector<T> &vector1, const Vector<T> &vector2)
	{
		assert(vector1.m_nElements == vector2.m_nElements);
		Vector<T> newVector = Vector<T>(vector1.m_nElements);
		
		for (int i = 0; i < vector1.m_nElements; i++)
		{
			newVector(i) = vector1.m_data[i] / vector2.m_data[i];
		}
		return newVector;
	};

	//T-vector operations:
	friend Vector<T> operator+(const T scalar, const Vector<T> &vector)
	{
		Vector<T> newVector = Vector<T>(vector.m_nElements);
		
		for (int i = 0; i < vector.m_nElements; i++)
		{
			newVector(i) = scalar + vector.m_data[i];
		}
		return newVector;
	};

	friend Vector<T> operator-(const T scalar, const Vector<T> &vector)
	{
		Vector<T> newVector = Vector<T>(vector.m_nElements);
		
		for (int i = 0; i < vector.m_nElements; i++)
		{
			newVector(i) = scalar - vector.m_data[i];
		}
		return newVector;
	};

	friend Vector<T> operator*(const T scalar, const Vector<T> &vector)
	{
		Vector<T> newVector = Vector<T>(vector.m_nElements);
		
		for (int i = 0; i < vector.m_nElements; i++)
		{
			newVector(i) = scalar * vector.m_data[i];
		}
		return newVector;
	};

	friend Vector<T> operator*(const Vector<T> &vector, const T scalar)
	{
		Vector<T> newVector = Vector<T>(vector.m_nElements);
		
		for (int i = 0; i < vector.m_nElements; i++)
		{
			newVector(i) = scalar * vector.m_data[i];
		}
		return newVector;
	};

	friend Vector<T> operator/(const T scalar, const Vector<T> &vector)
	{
		Vector<T> newVector = Vector<T>(vector.m_nElements);
		
		for (int i = 0; i < vector.m_nElements; i++)
		{
			newVector(i) = scalar / vector.m_data[i];
		}
		return newVector;
	};
};
#endif