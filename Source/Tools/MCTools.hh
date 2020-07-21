#ifndef MCTOOLS_HH
#define MCTOOLS_HH

#include <random>

#ifdef USEGEANT
#include <Eigen/Dense>
#endif

namespace MCTools
{
    void SetSeed(int seed);

    double RandDouble(double low, double high);

    double RandNorm(double mean, double sig);

    unsigned int RandPoisson(double mean);

    unsigned int RandDiscrete(const std::vector<double>& distro);

    std::vector<double> SampleNorm(double mean, double sig,
            unsigned int nSamples);

    std::vector<double> SampleUniform(double low, double high,
            unsigned int nSamples);

#ifdef USEGEANT
    Eigen::VectorXd RandNormNd(const Eigen::VectorXd& mean,
            const Eigen::MatrixXd& covar);

    Eigen::VectorXd RandSinhArcsinhNd(const Eigen::VectorXd& mean,
        const Eigen::VectorXd& covar, const Eigen::VectorXd& skew);
#endif

    static std::random_device rd;
    static std::mt19937 generator(rd());
}
#endif