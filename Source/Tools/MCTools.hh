#ifndef MCTOOLS_HH
#define MCTOOLS_HH

#include <random>

namespace MCTools
{
	void SetSeed(int seed);

	double RandDouble(double low, double high);

	std::vector<double> SampleNorm(double mean, double variance, unsigned int nSamples);

}
#endif