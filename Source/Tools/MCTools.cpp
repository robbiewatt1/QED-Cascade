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

double MCTools::RandNorm(double mean, double sig)
{
    std::normal_distribution<double> dist(mean, sig);
    return dist(generator);
}

unsigned int MCTools::RandPoisson(double mean)
{
    std::poisson_distribution<unsigned int> dist(mean);
    return dist(generator);
}

unsigned int MCTools::RandDiscrete(const std::vector<double>& distro)
{
    // Create CDF
    std::vector<double> cdf(distro.size());
    cdf[0] = distro[0];
    for(unsigned int i = 1; i < distro.size(); i++)
    {
        cdf[i] = cdf[i-1] + distro[i];

    }
    double rand = RandDouble(0.0, *(cdf.end() - 1));
    unsigned int sample(0);
    for (unsigned int i = 1; i < distro.size(); i++)
    {
        if (rand < cdf[i])
        {
            sample = i;
            break;
        }
    }
    return sample;
}

#ifdef USEGEANT
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

Eigen::VectorXd MCTools::RandSinhArcsinhNd(const Eigen::VectorXd& mean,
    const Eigen::VectorXd& covar, const Eigen::VectorXd& skew)
{
    // Convert covar to diagonal matrix
    Eigen::MatrixXd covarMatrix = covar.asDiagonal();

    // Sample multivariate normal
    Eigen::VectorXd sample = RandNormNd(mean, covarMatrix);

    double norm = 2.0 / std::sinh(std::asinh(2.0));

    return mean + covarMatrix * (Eigen::sinh(Eigen::asinh(sample.array())
                                 + skew.array())).matrix() * norm;
}
#endif

std::vector<double> MCTools::SampleNorm(double mean, double sig, unsigned int nSamples)
{

    std::normal_distribution<double> normDist(mean, sig); 
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
