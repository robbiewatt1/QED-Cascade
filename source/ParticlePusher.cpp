#include "ParticlePusher.hh"

ParticlePusher::ParticlePusher(double dt):
m_dt(dt), m_time(0)
{
}

ParticlePusher::~ParticlePusher()
{
}

void ParticlePusher::PushParticle(Particle &part)
{
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
		for (int i = 0; i < 3; i++)	// update each component using RK4 method
		{
			double posk1, posk2, posk3, posk4;
			
		}
	}

}

double ParticlePusher::PushPosition(double mass, double gamma, double P)
{
	return P / (mass * gamma)
}

double ParticlePusher::PushMomentum(double mass, double charge, double gamma,double P1, double P2,
									double E, double B1, double B2)
{
	return charge / (mass * gamma) * ( gamma * mass * E + (P1 * B2 - P2 * B1));
}