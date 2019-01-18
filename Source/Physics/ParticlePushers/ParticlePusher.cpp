#include "ParticlePusher.hh"

ParticlePusher::ParticlePusher(Field* field, double dt):
m_field(field), m_dt(dt), m_time(0)
{
}

ParticlePusher::~ParticlePusher()
{
}

void ParticlePusher::PushParticle(Particle &part)
{
	if (part.GetCharge() == 0)	// Particle is Chargless
	{
		// Only chargeless particles are photons. Propagate by dt each time step.
		// No need for RK4
		ThreeVector positionNew = part.GetPosition() + (m_dt / part.GetMomentum().Mag())
								 * part.GetMomentum();
		part.UpdateTrack(positionNew, part.GetMomentum());
		part.UpdateTime(m_dt);
	} else	// Particle is charged
	{
		ThreeVector posK1, posK2, posK3, posK4;
		ThreeVector momK1, momK2, momK3, momK4;
		ThreeVector eField, bField;

		m_field->GetField(part.GetTime(), part.GetPosition(), eField, bField);
		posK1 = PushPosition(part.GetMass(), part.GetMomentum());
		momK1 = PushMomentum(part.GetMass(), part.GetCharge(),
							 part.GetMomentum(),
							 eField, bField);
		
		m_field->GetField(part.GetTime() + m_dt / 2.0,
						  part.GetPosition() + posK1 * m_dt / 2.0,
						  eField, bField);
		posK2 = PushPosition(part.GetMass(), part.GetMomentum() + momK1 * m_dt / 2.0);
		momK2 = PushMomentum(part.GetMass(), part.GetCharge(), 
							 part.GetMomentum() + momK1 * m_dt / 2.0,
							 eField, bField);

		m_field->GetField(part.GetTime() + m_dt / 2.0,
						  part.GetPosition() + posK2 * m_dt / 2.0,
						  eField, bField);
		posK3 = PushPosition(part.GetMass(), part.GetMomentum() + momK2 * m_dt / 2.0);
		momK3 = PushMomentum(part.GetMass(), part.GetCharge(),
							 part.GetMomentum() + momK2 * m_dt / 2.0,
							 eField, bField);

		m_field->GetField(part.GetTime() + m_dt,
						  part.GetPosition() + posK2 * m_dt,
						  eField, bField);
		posK4 = PushPosition(part.GetMass(), part.GetMomentum() + momK3 * m_dt);
		momK4 = PushMomentum(part.GetMass(), part.GetCharge(),
							 part.GetMomentum() + momK3 * m_dt,
							 eField, bField);

		ThreeVector positionNew = part.GetPosition() + (m_dt / 6.0) 
								  * (posK1 + 2.0 * posK2 + 2.0 * posK3 + posK4);
		ThreeVector momentumNew = part.GetMomentum() + (m_dt / 6.0) 
								  * (momK1 + 2.0 * momK2 + 2.0 * momK3 + momK4);
		part.UpdateTrack(positionNew, momentumNew);
		part.UpdateTime(m_dt);
	}
}

void ParticlePusher::PushParticleList(ParticleList* partList)
{
	for (unsigned int i = 0; i < partList->GetNPart(); i++)
	{
		PushParticle(partList->GetParticle(i));
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
	ThreeVector newMomentum = charge * (Efield + (momentum.Cross(Bfield) / mass));
	return newMomentum;
}