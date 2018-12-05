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
	/*
	if (part.GetCharge() == 0)	// Particle is Chargless
	{
		// Only chargeless particles are photons. Propagate by p_i/P^2 in each direction
		double momMag2 = part.GetMomentum()[0] * part.GetMomentum()[0] +
							  part.GetMomentum()[1] * part.GetMomentum()[1] +
							  part.GetMomentum()[2] * part.GetMomentum()[2];
		std::vector<double> positionNew(3);
		positionNew[0] = part.GetPosition()[0] + m_dt * part.GetMomentum()[0] / momMag2;
		positionNew[1] = part.GetPosition()[1] + m_dt * part.GetMomentum()[1] / momMag2;
		positionNew[2] = part.GetPosition()[2] + m_dt * part.GetMomentum()[2] / momMag2;

		part.UpdatePosition(positionNew);
		
	} else	// Particle is charged
	{
		std::vector<double> positionNew(3);
		std::vector<double> momentumNew(3);

		std::vector<double> posk1(3), posk2(3), posk3(3), posk4(3);
		std::vector<double> momK1(3), momK2(3), momK3(3), momK4(3);
			
		posK1 = PushPosition(part.GetMass(), part.GetGamma(), part.GetMomentum()[i]);
		momK1 = PushMomentum(part.GetMass(), part.GetCharge(), part.GetGamma(), )

	}

}

std::vector<double> ParticlePusher::PushPosition(double mass, double gamma, const std::vector<double> &momentum)
{
	std::vector<double> UpdatePosition;
	for (unsigned int i = 0; i < 3; i++)
	{
		updatePos[i] = momentum[i] / (mass * gamma)
	}
	return P / (mass * gamma)
}

double ParticlePusher::PushMomentum(double mass, double charge, double gamma, double P1, double P2,
									double E, double B1, double B2)
{
	return charge * (E + (P1 * B2 - P2 * B1) / (mass * gamma));
}
*/