#include "ModifiedLandauPusher.hh"
#include <cmath>
#include "UnitsSystem.hh"

ModifiedLandauPusher::ModifiedLandauPusher(EMField* field, double dt):
ParticlePusher(field, dt)
{
}

ThreeVector ModifiedLandauPusher::PushMomentum(double mass, double charge,
    const ThreeVector &momentum, const ThreeVector &Efield,
    const ThreeVector &Bfield) const
{
    double gamma = std::sqrt(1.0 + momentum.Mag2() / (mass * mass));
    double beta  = std::sqrt(1.0 - 1.0 / (gamma * gamma));

    // calculate eta value
    ThreeVector partDir = momentum.Norm();
    ThreeVector ePara = Efield.Dot(partDir) * partDir;
    ThreeVector ePerp = Efield - ePara;
    
    double eta = std::sqrt((ePerp + beta * partDir.Cross(Bfield)).Mag2()
            + std::pow(partDir.Dot(Efield), 2.0) / (gamma * gamma)) * gamma;

    ThreeVector radReaction = (-2.0 / 3.0) * UnitsSystem::alpha * eta * eta
        * gaunt(eta) * partDir;

    ThreeVector newMomentum = charge * (Efield + (momentum.Cross(Bfield)
        / (mass * gamma))) + radReaction;

    return newMomentum;
}

double ModifiedLandauPusher::gaunt(double eta) const
{
    return std::pow(1.0 + 4.8 * (1.0 + eta) * std::log(1.0 + 1.7 * eta)
        + 2.44 * eta * eta, -2. / 3.);
}