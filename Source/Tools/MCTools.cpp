#include "MCTools.hh"
#include <random>
#include <iostream>

void MCTools::SetSeed(int seed)
{
    std::srand(seed);
}

double MCTools::RandDouble(double low, double high)
{
    std::uniform_real_distribution<double> dist(low, high);
    return dist(generator);
}

std::vector<double> MCTools::SampleNorm(double mean, double variance, unsigned int nSamples)
{

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

    std::uniform_real_distribution<double> dist(low, high); 
    std::vector<double> samples(nSamples);

    for (unsigned int i = 0; i < nSamples; i++)
    {
        samples[i] = dist(generator);
    }
    return samples;
}
