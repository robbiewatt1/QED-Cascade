#include <cmath>
#include <iostream>

#include "Numerics.hh"

double Numerics::SimpsonsRule(const std::vector<double> &axis, const std::vector<double> &integrand)
{

	double deltaX = (axis.back() - axis[0]) / (axis.size() - 1.0);
	double integral = integrand[0];

	for (unsigned int i = 1; i < integrand.size() - 1; i += 2)
	{
		integral += 4.0 * integrand[i];
	}
	for (unsigned int i = 2; i < integrand.size() - 2; i += 2)
	{
		integral += 2.0 * integrand[i];
	}
	integral += integrand.back();

	return (integral * deltaX / 3.0);
}


double Numerics::Interpolate1D(double* samplePoints, double* sampleData, unsigned int sampleSize,
					 		   double queryPoint)
{
	double queryData;
	if (queryPoint < samplePoints[0])
	{
		// value is not within the range so we need to use extrapolation
		queryData = sampleData[0] + (sampleData[1] - sampleData[0]) * ((queryPoint - samplePoints[0])
									/ (samplePoints[1] - samplePoints[0]));
	} else if (queryPoint > samplePoints[sampleSize-1])
	{
		// value is not within the range so we need to use extrapolation
		int end = sampleSize - 1;
		queryData = sampleData[end] + (sampleData[end] - sampleData[end-1]) 
									  * ((queryPoint - samplePoints[end-1])
		               				  / (samplePoints[end] - samplePoints[end-1]));
	} else
	{
		int lIndex(0), hIndex(1);
		for (unsigned int i = 0; i < sampleSize; i++)
		{
			if (queryPoint < samplePoints[i])
			{
				lIndex = i - 1;
				hIndex = i;
				break;
			}
		}
		queryData = sampleData[lIndex] + (queryPoint - samplePoints[lIndex]) 
											* (sampleData[hIndex] - sampleData[lIndex])
				    				 		/ (samplePoints[hIndex] - samplePoints[lIndex]);
	}
	return queryData;
}

int Numerics::ArrayIndex(double* samplePoints, unsigned int sampleSize, double queryPoint)
{
	unsigned int lowIndex = 0;
	for (unsigned int i = 0; i < sampleSize; i++)
	{
		if (samplePoints[i] < queryPoint)
		{
			lowIndex = i;
		} else
		{
			break;
		}
	}
	if (lowIndex < sampleSize - 1)
	{
		if ( queryPoint - samplePoints[lowIndex] < samplePoints[lowIndex+1] - queryPoint)
		{
			return lowIndex;
		} else
		{
			return lowIndex+1;
		}
	} else
	{
		return sampleSize - 1;
	}
}
