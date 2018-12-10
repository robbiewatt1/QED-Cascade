#include "LaserField.hh"
#include "Particle.hh"
#include "ThreeVector.hh"
#include "ParticlePusher.hh"
#include "HDF5Output.hh"
#include "ThreeMatrix.hh"

int main(int argc, char* argv[])
{
	Particle electron = Particle(1, -1, true);
	electron.UpdatePosition(ThreeVector(0, 0, 0));
	electron.UpdateMomentum(ThreeVector(10, 10, 10));

	LaserField* field = new LaserField(1.0, 2.0, 1.0, 20, 0, ThreeVector(0,0,0), ThreeVector(0,30,30));
	ParticlePusher pusher = ParticlePusher(field, 0.01);
	for (int i = 0; i < 1000; ++i)
	{
		pusher.PushParticle(electron);
	}
	HDF5Output file = HDF5Output("./Data/test.h");
	electron.SaveTrack(file);


	std::vector<double> xax(100);
	for (int i = 0; i < 100; ++i)
	{
		xax[i] = -5.0 + (double)i /10.0;
	}
	std::vector<double> time(20);
	for(int i = 0; i < 20; i++)
	{
		time[i] = (double) i;
	}
	field->SaveField(file, time, xax, xax, xax);
	return 0;
}
