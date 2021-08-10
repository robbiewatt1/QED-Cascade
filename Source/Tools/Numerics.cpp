#include <cmath>
#include <iostream>

#include "Numerics.hh"

double Numerics::SimpsonsRule(const std::vector<double> &axis, const std::vector<double> &integrand)
{

    double deltaX = (axis.back() - axis[0]) / (axis.size() - 1.0);
    double integral = integrand[0];

    for (unsigned int i = 1; i < integrand.size() - 1; i += 2)
    {
        integral += 4.0 * integrand[i];
    }
    for (unsigned int i = 2; i < integrand.size() - 2; i += 2)
    {
        integral += 2.0 * integrand[i];
    }
    integral += integrand.back();

    return (integral * deltaX / 3.0);
}


double Numerics::Interpolate1D(double* samplePoints, double* sampleData, unsigned int sampleSize,
                               double queryPoint)
{
    double queryData;
    if (queryPoint < samplePoints[0])
    {
        // value is not within the range so we need to use extrapolation
        queryData = sampleData[0] + (sampleData[1] - sampleData[0]) * ((queryPoint - samplePoints[0])
                                    / (samplePoints[1] - samplePoints[0]));
    } else if (queryPoint > samplePoints[sampleSize-1])
    {
        // value is not within the range so we need to use extrapolation
        int end = sampleSize - 1;
        queryData = sampleData[end] + (sampleData[end] - sampleData[end-1]) 
                                      * ((queryPoint - samplePoints[end-1])
                                      / (samplePoints[end] - samplePoints[end-1]));
    } else
    {
        int lIndex(0), hIndex(1);
        for (unsigned int i = 0; i < sampleSize; i++)
        {
            if (queryPoint < samplePoints[i])
            {
                lIndex = i - 1;
                hIndex = i;
                break;
            }
        }
        queryData = sampleData[lIndex] + (queryPoint - samplePoints[lIndex]) 
                                            * (sampleData[hIndex] - sampleData[lIndex])
                                            / (samplePoints[hIndex] - samplePoints[lIndex]);
    }
    return queryData;
}

double Numerics::interpolate2D(double* sampleX, double* sampleY, double** sampleZ,
    unsigned int sampleSize[2], double queryPoint[2])
{
    if (sampleSize[0] == 1 && sampleSize[1] == 1)
    {
        return sampleZ[0][0];
    } else
    {
        // Get the closest points
        unsigned int closestIndexX[2];
        if (queryPoint[0] < sampleX[0])
        {
            closestIndexX[0] = 0;
            closestIndexX[1] = 1;
        } else if (queryPoint[0] > sampleX[sampleSize[0] - 1])
        {
            closestIndexX[0] = sampleSize[0] - 2;
            closestIndexX[1] = sampleSize[0] - 1;      
        } else
        {
            for (int i = 0; i < sampleSize[0]; i++)
            {
                if(sampleX[i] > queryPoint[0])
                {
                    closestIndexX[0] = i - 1;
                    closestIndexX[1] = i;
                    break;
                }
            }
        }
        unsigned int closestIndexY[2];
        if (queryPoint[1] < sampleY[0])
        {
            closestIndexY[0] = 0;
            closestIndexY[1] = 1;
        } else if (queryPoint[1] > sampleX[sampleSize[1] - 1])
        {
            closestIndexY[0] = sampleSize[1] - 2;
            closestIndexY[1] = sampleSize[1] - 1;
        } else
        {
            for (int i = 0; i < sampleSize[1]; i++)
            {
                if(sampleY[i] > queryPoint[1])
                {
                    closestIndexY[0] = i - 1;
                    closestIndexY[1] = i;
                    break;
                }
            }
        }
        double x1 = sampleX[closestIndexX[0]];
        double x2 = sampleX[closestIndexX[1]];
        double y1 = sampleY[closestIndexY[0]];
        double y2 = sampleY[closestIndexY[1]];
        double z11 = sampleZ[closestIndexX[0]][closestIndexY[0]];
        double z12 = sampleZ[closestIndexX[0]][closestIndexY[1]];
        double z21 = sampleZ[closestIndexX[1]][closestIndexY[0]];
        double z22 = sampleZ[closestIndexX[1]][closestIndexY[1]];  
        return 1.0 / ((x2 - x1) * (y2 - y1))
            * (z11 * (x2 - queryPoint[0]) * (y2 - queryPoint[1])
             + z21 * (queryPoint[0] - x1) * (y2 - queryPoint[1])
             + z12 * (x2 - queryPoint[0]) * (queryPoint[1] - y1)
             + z22 * (queryPoint[0] - x1) * (queryPoint[1] - y1));
    }
}

int Numerics::ArrayIndex(double* samplePoints, unsigned int sampleSize, double queryPoint)
{
    unsigned int lowIndex = 0;
    for (unsigned int i = 0; i < sampleSize; i++)
    {
        if (samplePoints[i] < queryPoint)
        {
            lowIndex = i;
        } else
        {
            break;
        }
    }
    if (lowIndex < sampleSize - 1)
    {
        if ( queryPoint - samplePoints[lowIndex] < samplePoints[lowIndex+1] - queryPoint)
        {
            return lowIndex;
        } else
        {
            return lowIndex + 1;
        }
    } else
    {
        return sampleSize - 1;
    }
}

void Numerics::ClosestPoints(double* samplePoints, unsigned int sampleSize,
    double queryPoint, int& lowIndex, double& frac)
{
    lowIndex = 0;
    for (unsigned int i = 0; i < sampleSize; i++)
    {
        if (samplePoints[i] < queryPoint)
        {
            lowIndex = i;
        } else
        {
            break;
        }
    }
    if (lowIndex < sampleSize - 2)
    {
        frac = (queryPoint - samplePoints[lowIndex]) / (samplePoints[lowIndex+1]
            - samplePoints[lowIndex]);
    } else
    {
        std::cerr << "Error: Tables can't handle your extreme simulation."
            << std::endl;
        std::exit(-1);
    }
}