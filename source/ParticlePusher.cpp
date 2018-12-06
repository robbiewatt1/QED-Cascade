#include "ParticlePusher.hh"

ParticlePusher::ParticlePusher(LaserField* field, double dt):
m_dt(dt), m_time(0), m_field(field)
{
}

ParticlePusher::~ParticlePusher()
{
}

void ParticlePusher::PushParticle(Particle &part)
{
	if (part.GetCharge() == 0)	// Particle is Chargless
	{
		// Only chargeless particles are photons. Propagate by p_i/P^2 in each direction.
		// For some reason this isn't RK4 yet
		ThreeVector positionNew = part.GetPosition() + (m_dt / part.GetMomentum().Mag2())
								 * part.GetMomentum();
		part.UpdatePosition(positionNew);
	} else	// Particle is charged
	{
		ThreeVector posK1, posK2, posK3, posK4;
		ThreeVector momK1, momK2, momK3, momK4;
		posK1 = PushPosition(part.GetMass(), part.GetMomentum());
		momK1 = PushMomentum(part.GetMass(), part.GetCharge(), posK1,
							 m_field->GetEfield(part.GetPosition()),
							 m_field->GetBfield(part.GetPosition()));
		posK2 = PushPosition(part.GetMass(), part.GetMomentum() + momK1 * m_dt / 2.0);
		momK2 = PushMomentum(part.GetMass(), part.GetCharge(), posK2,
							 m_field->GetEfield(part.GetPosition() + posK1 * m_dt / 2.0),
							 m_field->GetBfield(part.GetPosition() + posK1 * m_dt / 2.0));
		posK3 = PushPosition(part.GetMass(), part.GetMomentum() + momK2 * m_dt / 2.0);
		momK3 = PushMomentum(part.GetMass(), part.GetCharge(), posK3,
							 m_field->GetEfield(part.GetPosition() + posK2 * m_dt / 2.0),
							 m_field->GetBfield(part.GetPosition() + posK2 * m_dt / 2.0));
		posK4 = PushPosition(part.GetMass(), part.GetMomentum() + momK3 * m_dt);
		momK4 = PushMomentum(part.GetMass(), part.GetCharge(), posK4,
							 m_field->GetEfield(part.GetPosition() + posK3 * m_dt),
							 m_field->GetBfield(part.GetPosition() + posK3 * m_dt));

		ThreeVector positionNew = part.GetPosition() + (m_dt / 6.0) 
								  * (posK1 + 2.0 * posK2 + 2.0 * posK3 + posK4);
		ThreeVector momentumNew = part.GetMomentum() + (m_dt / 6.0) 
								  * (momK1 + 2.0 * momK2 + 2.0 * momK3 + momK4);
		part.UpdatePosition(positionNew);
		part.UpdateMomentum(positionNew);
	}
}

ThreeVector ParticlePusher::PushPosition(double mass, const ThreeVector &momentum)
{
	double gamma  = std::sqrt(1.0 + momentum.Mag2() / (mass * mass));
	ThreeVector newPosition = momentum / (mass * gamma);
	return newPosition;
}

ThreeVector ParticlePusher::PushMomentum(double mass, double charge, const ThreeVector &momentum,
										 const ThreeVector &Efield, const ThreeVector &Bfield)
{
	double gamma  = std::sqrt(1.0 + momentum.Mag2() / (mass * mass));
	ThreeVector newMomentum = charge * (Efield + (momentum.Cross(Bfield) / (mass * gamma)));
	return newMomentum;
}