#ifndef PLANEFIELD_HH
#define PLANEFIELD_HH

#include "Field.hh"
#include "ThreeVector.hh"
#include "ThreeMatrix.hh"

class PlaneField: public Field
{
public:

	PlaneField();

	PlaneField(double maxE, double waveLength, double polAngle,
			   const ThreeVector& direction);
	
	virtual ~PlaneField();

//	void SaveField(HDF5Output &file, const std::vector<double> &tAxis,
//				   const std::vector<double> &xAxis, const std::vector<double> &yAxis,
//				   const std::vector<double> &zAxis);

	virtual void GetField(double time, const ThreeVector &position,
						  ThreeVector &eField, ThreeVector &bField) const;

private:
	double m_maxE;	// Beam max intensity
	double m_waveLength;	// wavelength
	double m_polAngle; // Polerisation angle
	double m_waveNum;	// Wavenumber of wave

	ThreeMatrix m_rotaion;	// Matrix to rotate so beam goes inb z axis
	ThreeMatrix m_rotationInv;	// Matrix to rotate back
};
#endif