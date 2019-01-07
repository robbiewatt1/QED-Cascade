#include "LaserField.hh"
#include "Particle.hh"
#include "ThreeVector.hh"
#include "ParticlePusher.hh"
#include "HDF5Output.hh"
#include "ThreeMatrix.hh"
#include "ParticleList.hh"
#include "MCTools.hh"
#include <fstream>

int main(int argc, char* argv[])
{
	// Set up the laser field
	LaserField* field = new LaserField(100.0, 10.0, 1.0, 2.0, 0.0, 0.0, 
					   ThreeVector(0,0,0), ThreeVector(0,0,1));

	// Set up the primary and secondary particle lists
	ParticleList* primaries   = new ParticleList();
	ParticleList* secondaries = new ParticleList(); 
	primaries->GenericSource(100, 1, 1, 10, 1, ThreeVector(0,0,-0.5), ThreeVector(0,0,-1));

	// Set the particle pusher
	ParticlePusher* pusher = new ParticlePusher(field, 0.001);

	// Set the output file
	HDF5Output* file = new HDF5Output("./Data/test.h");

	// Enter the main pushing loop
	int n = 0;
	while(true)
	{
		// Push primary particles
		for (unsigned int i = 0; i < primaries->GetNPart(); i++)
		{
			pusher->PushParticle(primaries->GetParticle(i));
		}
		// Push secondary particles
		for (unsigned int i = 0; i < secondaries->GetNPart(); i++)
		{
			pusher->PushParticle(secondaries->GetParticle(i));
		}

		// add up optical depth and check if process occurs
		if (n == 10000)
		{
			break;
		}
		n++;
	}
	primaries->SaveTracks(file);

	// define time using laser frequency with 
/*
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
*/
	return 0;
}
