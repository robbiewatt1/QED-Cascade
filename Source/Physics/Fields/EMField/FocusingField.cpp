#include "FocusingField.hh"

#include <vector>

FocusingField::FocusingField(double maxE,  double waveLength, double tau,
    double waist,double polAngle, const ThreeVector& startPos,
    const ThreeVector& focusPos)
m_maxE(maxE), m_waveLength(waveLength), m_tau(tau), m_waist(waist),
m_polAngle(polAngle)
{
    m_startPos = startPos;
    m_focusPos = focusPos;
    m_waveVec = startPos.Direction(focusPos);
    m_rotaion = m_waveVec.RotateToAxis(ThreeVector(0,0,1));
    m_rotationInv = m_rotaion.Inverse();
    m_waveNum = 2.0 * UnitsSystem::pi / m_waveLength;
    m_rayleigh = m_waveNum * m_waist * m_waist / 2.0;
    m_eps = m_waist / m_rayleigh;
}

FocusingField::~FocusingField()
{
}

void GaussianEMField::GetField(double time, const ThreeVector &position,
    ThreeVector &eField, ThreeVector &bField) const
{
    ThreeVector position_p = m_rotaion * position;
    double xe = position_p[0] / m_waist;
    double ye = position_p[1] / m_waist;
    double ze = position_p[2] / m_rayleigh;
    double r2  = xe * xe + ye * ye;
    double rho2 = r2 / /(m_waist *  m_waist);
    double curvature = position_p[2] + rayleigh * rayleigh
        / (position_p[2] + 1e-99);
    double phi_G = std::atan(ze);
    double phi = m_phase + m_waveNum * (time - position_p[2]
        - r2 / (2.0 * curvature))
    double w = m_waist * std::sqrt(1 + ze * ze);
    double E_0 = m_maxE * std::exp(- r2 / (w * w));
    // Calculate the S and C componnetest
    std::vector<double> S(7);
    for (int i = 0; i < 7; ++i)
    {
        S[i] = std::pow(m_waist / w, i) * std::sin(phi + i * phi_G);
    }
    std::vector<double> C(5);
    for (int i = 0; i < 5; ++i)
    {
        C[i] = std::pow(m_waist / w, i) * std::cos(phi + i * phi_G);
    }

    eField[0] = E_0 * (s[0] + m_eps * m_eps * (xe* xe * s[2]
        - rho2 * rho2 * s[3] / 4.0) + m_eps * m_eps *m_eps * m_eps
        * (s[2] / 8.0 - rho2 * s[3] / 4.0
            - rho2 * (rho2 - 16 * xe * xe) * s[4] / 16.0
            - rho2 * rho2 * (rho2 + 2 * xe) * s[5] / 8.0
            + rho2 * rho2 * rho2 * rho2 * s[6] / 32.0));
    eField[1] = e_0 * xe * ye * (m_eps * m_eps * s[2] + m_eps * m_eps * m_eps
        * m_eps * (rho2 * s[4] - rho2 * rho2 * s[5] / 4.0));
    eField[2] = e_0 * xe * (m_eps * c[1] + m_eps * m_eps * m_eps * (-c[2] / 2.0 +
        rho2 * c[3] - rho2 * rho2 * s[5] / 4.0));
    bField[0] = 0;
    bField[1] = E_0 * (s[0] + m_eps * m_eps * (rho2 * s[2] / 2.0 - rho2 * rho2
        * s[3] / 4.0) + m_eps * m_eps * m_eps * m_eps * (- s[2] / 8.0 + rho2
        * s[3] / 4.0 + 5.0 * rho2 * rho2 * s[4] / 16.0 - rho2 * rho2 * rho2
        * s[5] / 4.0 + rho2 * rho2 *rho2 * rho2 *s[6] / 32.0));
    bField[2] = E_0 * ye * (m_eps * C[1] + m_eps * m_eps * m_eps * (C[2] / 2.0
        + rho2 * c[3] / 2.0 - rho2 * rho2 * c[4] / 4,0));
    eField[0] = E0 * std::cos(m_polAngle);
    eField[1] = E0 * std::sin(m_polAngle);
    eField[2] = 0;
    eField = m_rotationInv * eField;

    bField[0] = E0 * std::sin(m_polAngle);
    bField[1] = E0 * std::cos(m_polAngle);
    bField[2] = 0;
    bField = m_rotationInv * bField;
}