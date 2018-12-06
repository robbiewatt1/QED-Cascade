#include "LaserField.hh"
#include "Particle.hh"
#include "ThreeVector.hh"
#include "ParticlePusher.hh"
#include "HDF5Output.hh"

int main(int argc, char* argv[])
{
	Particle electron = Particle(1, -1, true);
	electron.UpdatePosition(ThreeVector(0, 0, 0));
	electron.UpdateMomentum(ThreeVector(0, 1, 0));
	LaserField* field;
	ParticlePusher pusher = ParticlePusher(field, 0.001);
	for (int i = 0; i < 100; ++i)
	{
		pusher.PushParticle(electron);
	}
	HDF5Output file = HDF5Output("./Data/test.h");
	electron.SaveTrack(file);
	return 0;
}
