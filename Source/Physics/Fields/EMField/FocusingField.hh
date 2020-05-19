#ifndef FoucusingField_hh
#define FoucusingField_hh

#include "EMField.hh"
#include "ThreeVector.hh"
#include "ThreeMatrix.hh"


/* Based on Fields of a Gaussian beam beyond the paraxial approximation
by y.i. salamin going as far as the 4th term in epsilon */

class FocusingField: public EMField
{
public:
    FocusingField(double maxE,  double waveLength, double tau, double waist,
        double polAngle, double phase, const ThreeVector& startPos,
        const ThreeVector& focusPos);
 
    virtual ~FocusingField();

    void GetField(double time, const ThreeVector &position, ThreeVector &eField,
        ThreeVector &bField) override const;

    double m_maxE;  // Beam max intensity
    double m_waveLength;    // wavelength
    double m_tau;   // duration of beam
    double m_waist; // Waist of the beam at focus
    double m_polAngle; // Polerisation angle
    double m_phase;    // Phase of pulse
    double m_waveNum;   // Wave number

    ThreeMatrix m_rotaion;  // Matrix to rotate so beam goes inb z axis
    ThreeMatrix m_rotationInv;  // Matrix to rotate back
    ThreeVector m_waveVec; // Wave vector of beam
    ThreeVector m_startPos; // Start positipon of laser
    ThreeVector m_focusPos; // focus point of beam
};


#endif