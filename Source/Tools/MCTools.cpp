#include "MCTools.hh"
#include <random>

void MCTools::SetSeed(int seed)
{
	std::srand(seed);
}

double MCTools::RandDouble(double low, double high)
{
	long long unsigned int maxInt = ((long unsigned int)RAND_MAX + 1)
   							    	* ((long unsigned int)RAND_MAX + 1);
    long long unsigned int randInt = std::rand() * ((long unsigned int)RAND_MAX + 1) 
    								 + std::rand();
    
    return low + (double)randInt / maxInt * (high - low);
}

std::vector<double> MCTools::SampleNorm(double mean, double variance, unsigned int nSamples)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::normal_distribution<double> normDist(mean, variance); 
	std::vector<double> samples(nSamples);

	for (unsigned int i = 0; i < nSamples; i++)
	{
		samples[i] = normDist(generator);
	}
	return samples;
}

std::vector<double> MCTools::SampleUniform(double low, double high, unsigned int nSamples)
{
	std::vector<double> samples(nSamples);
	for (unsigned int i = 0; i < nSamples; i++)
	{
		samples[i] = RandDouble(low, high);
	}
	return samples;
}
