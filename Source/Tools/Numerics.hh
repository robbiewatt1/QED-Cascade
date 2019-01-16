#ifndef NUMERICS_HH
#define NUMERICS_HH

#include <vector>

namespace Numerics
{
/* Interhgartion using the simpsons rule. Requires axis to be an evenly spaced vector.
*/
double SimpsonsRule(const std::vector<double> &axis, const std::vector<double> &integrand);

// 1D linear interpolation. if the quere points are outside of the range of know points then
// a linear extrapolation method will be used
//double Interpolate1D(const Vector<double> &samplePoints, const Vector<double> &sampleData,
//                             const Vector<double> &queryPoints);
double Interpolate1D(double* samplePoints, double* sampleData, unsigned int sampleSize, 
					 double queryPoint);
}
#endif