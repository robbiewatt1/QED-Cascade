#include "MCTools.hh"

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
    
    return low + (double)maxInt / maxInt * (high - low);
}

double MCTools::SampleNorm(double mean, double variance)
{
	return mean + std::sqrt(2.0 * variance * std::log(RandDouble(0,1)));
}