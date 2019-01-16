#ifndef LASERFIELD_HH
#define LASERFIELD_HH

#include <vector>

#include "Field.hh"
#include "ThreeVector.hh"
#include "ThreeMatrix.hh"

class LaserField: public Field
{
public:

	LaserField();

	LaserField(double maxI, double tau, double waveLength, double waist, double start,
			   double polAngle, const ThreeVector &waveNum, const ThreeVector &focus);
	
	virtual ~LaserField();

//	void SaveField(HDF5Output &file, const std::vector<double> &tAxis,
//				   const std::vector<double> &xAxis, const std::vector<double> &yAxis,
//				   const std::vector<double> &zAxis);

	virtual void GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const;

//	ThreeVector GetEfield(const ThreeVector &position, double time) const;

//	ThreeVector GetBfield(const ThreeVector &position, double time) const;

private:
	double m_maxI;	// Beam max intensity
	double m_tau;	// beam duration
	double m_waveLength;	// wavelength
	double m_waist;	// Waist of the beam at focus
	double m_start;	// distance from focus of start of beam
	double m_polAngle; // Polerisation angle
	double m_waveNum;	// Wave number


	ThreeMatrix m_rotaion;	// Matrix to rotate so beam goes inb z axis
	ThreeMatrix m_rotationInv;	// Matrix to rotate back
	ThreeVector m_waveVec; // Wave vector of beam
	ThreeVector m_focus;	// focus point of beam
};
#endif