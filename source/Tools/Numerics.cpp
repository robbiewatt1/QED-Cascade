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


double Numerics::Interpolate1D(const std::vector<double> &samplePoints, 
							   const std::vector<double> &sampleData,
							   double queryPoint)
{
	double queryData;
	if (queryPoint < samplePoints[0])
	{
		// value is not within the range so we need to use extrapolation
		queryData = sampleData[0] + (sampleData[1] - sampleData[0]) * ((queryPoint - samplePoints[0])
									/ (samplePoints[1] - samplePoints[0]));
	} else if (queryPoint > samplePoints.back())
	{
		// value is not within the range so we need to use extrapolation
		int end = samplePoints.size() - 1;
		queryData = sampleData[end] + (sampleData[end] - sampleData[end-1]) 
									  * ((queryPoint - samplePoints[end-1])
		               				  / (samplePoints[end] - samplePoints[end-1]));
	} else
	{
		int lIndex, hIndex;
		for (unsigned int i = 0; i < samplePoints.size(); i++)
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
