#include "LaserField.hh"
#include "Particle.hh"
#include "ThreeVector.hh"
#include "ParticlePusher.hh"
#include "HDF5Output.hh"
#include "ThreeMatrix.hh"
#include "ParticleList.hh"

int main(int argc, char* argv[])
{
	// define time using laser frequency with 

	Particle electron = Particle(1, -1, true);
	electron.UpdateTrack(ThreeVector(0.0, 0.0, 0.0), ThreeVector(0.0, 0.0, 0.0));

	LaserField* field = new LaserField(100.0, 2.0, 1.0, 2.0, -8.0, 0.0, ThreeVector(0,0,0), ThreeVector(0,0,1));
	ParticlePusher pusher = ParticlePusher(field, 0.001);
	for (int i = 0; i < 100000; ++i)
	{
		pusher.PushParticle(electron);
	}
	HDF5Output file = HDF5Output("./Data/test.h");
	electron.SaveTrack(file);


	std::vector<double> zax(500);
	for (int i = 0; i < 500; ++i)
	{
		zax[i] = -20.0 + (double)i / 12.5;
	}
	std::vector<double> xax(3);
        for (int i = 0; i < 3; ++i)
        {
                xax[i] = -5.0 + (double)i / 0.3;
        }
	std::vector<double> time(20);
	for(int i = 0; i < 20; i++)
	{
		time[i] = (double) i;
	}
	field->SaveField(file, time, zax, zax, zax);

	return 0;
}
