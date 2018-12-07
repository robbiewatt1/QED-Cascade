#ifndef LASERFIELD_HH
#define LASERFIELD_HH

#include <vector>

#include "ThreeVector.hh"
#include "HDF5Output.hh"

class LaserField
{
public:

	LaserField();

	LaserField(double maxI, double tau, double lambda, double waist, const ThreeVector &waveNum);
	
	~LaserField();

	void SaveField(HDF5Output &file, double initT, double endT, const std::vector<double> &xAxis,
				   const std::vector<double> &yAxis, const std::vector<double> &zAxis);

	ThreeVector GetEfield(const ThreeVector &position, double time) const;

	ThreeVector GetBfield(const ThreeVector &position, double time) const;


private:

	double BeamWaist(double z) const;

private:
	double m_maxI;	// Beam max intensity
	double m_tau;	// beam duration
	double m_lambda;	// wavelength
	double m_waist;	// Waist of the beam at focus
	double m_polAngle; // Polerisation angle

	ThreeVector m_waveNum;


};
#endif