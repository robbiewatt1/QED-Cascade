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
	for (int i = 0; i < 100; ++i)
	{
		ParticlePusher pusher = ParticlePusher(field, 0.001);
	}
	std::cerr << "here" << std::endl;
	HDF5Output file = HDF5Output("./Data/test.h");
	std::cerr << "here" << std::endl;
	electron.SaveTrack(file);
	return 0;
}
