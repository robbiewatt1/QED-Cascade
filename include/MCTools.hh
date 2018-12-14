#ifndef MCTOOLS_HH
#define MCTOOLS_HH

#include <random>

namespace MCTools
{
	void SetSeed(int seed);

	double RandDouble(double low, double high);

	double SampleNorm(double mean, double variance);

}