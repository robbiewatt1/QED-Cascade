#ifndef MCTOOLS_HH
#define MCTOOLS_HH

#include <random>
#include <Eigen/Dense>

namespace MCTools
{
    void SetSeed(int seed);

    double RandDouble(double low, double high);

    double RandNorm(double mean, double variance);

    unsigned long int RandPoisson(double mean);

    Eigen::VectorXd RandNormNd(const Eigen::VectorXd& mean,
            const Eigen::MatrixXd& covar);

    Eigen::VectorXd RandSinhArcsinhNd(Eigen::VectorXd mean, Eigen::VectorXd covar,
            Eigen::VectorXd skew);

    std::vector<double> SampleNorm(double mean, double variance,
            unsigned int nSamples);

    std::vector<double> SampleUniform(double low, double high,
            unsigned int nSamples);

    static std::random_device rd;
    static std::mt19937 generator(rd());
}
#endif