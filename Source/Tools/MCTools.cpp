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

double MCTools::RandNorm(double mean, double variance)
{
    std::normal_distribution<double> dist(mean, variance);
    return dist(generator);
}

unsigned long int MCTools::RandPoisson(double mean)
{
    std::poisson_distribution<unsigned long int> dist(mean);
    return dist(generator);
}

Eigen::VectorXd MCTools::RandNormNd(const Eigen::VectorXd& mean,
        const Eigen::MatrixXd& covar)
{
    // Generate vector of random
    int size = mean.rows();
    std::vector<double> sample = SampleNorm(0., 1., size);

    Eigen::Map<Eigen::VectorXd> ei_sample(sample.data(), size);

    // Try and cholesky decomposition, if not PD then use eigen solver
    Eigen::MatrixXd lMatrix(size, size);
    Eigen::LLT<Eigen::MatrixXd> cholSolver(covar);
    if (cholSolver.info()==Eigen::Success)
    {
        // Use cholesky solver
        lMatrix = cholSolver.matrixL();
    } else 
    {
        // Use eigen solver
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigenSolver(covar);
        lMatrix = eigenSolver.eigenvectors() 
                   * eigenSolver.eigenvalues().cwiseSqrt().asDiagonal();
    }

    return mean + lMatrix * ei_sample;
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
