#include "LaserField.hh"
#include "Particle.hh"
#include "ThreeVector.hh"
#include "ParticlePusher.hh"
#include "HDF5Output.hh"
#include "ThreeMatrix.hh"

int main(int argc, char* argv[])
{
	// define time using laser frequency with 

	Particle electron = Particle(1, -1, true);
	electron.UpdateTrack(ThreeVector(0.0, 0.0, 0.0), ThreeVector(0.0, 0.0, 0.0));

	LaserField* field = new LaserField(20.0, 2.0, 2.0, 10.0, 0.0, ThreeVector(0,0,0), ThreeVector(0,0,10000));
	ParticlePusher pusher = ParticlePusher(field, 0.0001);
	for (int i = 0; i < 100000; ++i)
	{
		pusher.PushParticle(electron);
	}
	HDF5Output file = HDF5Output("./Data/test.h");
	electron.SaveTrack(file);


	std::vector<double> zax(400);
	for (int i = 0; i < 400; ++i)
	{
		zax[i] = -5.0 + (double)i / 40.0;
	}
	std::vector<double> xax(3);
        for (int i = 0; i < 3; ++i)
        {
                xax[i] = -5.0 + (double)i / 0.3;
        }
	std::vector<double> time(50);
	for(int i = 0; i < 50; i++)
	{
		time[i] = (double) i;
	}
	field->SaveField(file, time, xax, zax, zax);

	return 0;
}
