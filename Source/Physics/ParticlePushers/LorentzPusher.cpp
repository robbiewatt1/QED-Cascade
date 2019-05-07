#include "LorentzPusher.hh"

LorentzPusher::LorentzPusher(EMField* field, double dt):
ParticlePusher(field, dt)
{
}

ThreeVector LorentzPusher::PushMomentum(double mass, double charge, const ThreeVector &momentum,
		const ThreeVector &Efield, const ThreeVector &Bfield) const
{
    double gamma = std::sqrt(1.0 + momentum.Mag2() / (mass * mass));
    ThreeVector newMomentum = charge * (Efield + (momentum.Cross(Bfield) / (mass * gamma)));
    return newMomentum;
}