#ifndef PlaneEMField_HH
#define PlaneEMField_HH

#include "EMField.hh"
#include "ThreeVector.hh"
#include "ThreeMatrix.hh"

class PlaneEMField: public EMField
{
public:

    PlaneEMField();

    PlaneEMField(double maxE, double waveLength, double polAngle,
               const ThreeVector& direction);
    
     ~PlaneEMField();

    void GetField(double time, const ThreeVector &position,
                  ThreeVector &eField, ThreeVector &bField) const override;

private:
    double m_maxE;          // Beam max intensity
    double m_waveLength;    // wavelength
    double m_polAngle;      // Polerisation angle
    double m_waveNum;       // Wavenumber of wave

    ThreeMatrix m_rotaion;       // Matrix to rotate so beam goes in z
    ThreeMatrix m_rotationInv;  // Matrix to rotate back
};
#endif