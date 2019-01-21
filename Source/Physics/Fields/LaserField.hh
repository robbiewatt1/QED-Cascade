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

	LaserField(double maxE, double waveLength, double tau, double waist, double polAngle,
			   const ThreeVector& startPos, const ThreeVector& focusPos);
	
	virtual ~LaserField();

//	void SaveField(HDF5Output &file, const std::vector<double> &tAxis,
//				   const std::vector<double> &xAxis, const std::vector<double> &yAxis,
//				   const std::vector<double> &zAxis);

	virtual void GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const;

private:
	double m_maxE;	// Beam max intensity
	double m_waveLength;	// wavelength
	double m_tau;	// beam duration	
	double m_waist;	// Waist of the beam at focus
	double m_polAngle; // Polerisation angle
	double m_waveNum;	// Wave number


	ThreeMatrix m_rotaion;	// Matrix to rotate so beam goes inb z axis
	ThreeMatrix m_rotationInv;	// Matrix to rotate back
	ThreeVector m_waveVec; // Wave vector of beam
	ThreeVector m_startPos;	// Start positipon of laser
	ThreeVector m_focusPos;	// focus point of beam
};
#endif