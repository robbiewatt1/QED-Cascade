#include "LaserField.hh"
#include "Particle.hh"
#include "ThreeVector.hh"
#include "ParticlePusher.hh"
#include "HDF5Output.hh"
#include "ThreeMatrix.hh"
#include "ParticleList.hh"
#include "MCTools.hh"
#include "NonLinearCompton.hh"
#include <fstream>

int main(int argc, char* argv[])
{
	// Set up the laser field
	LaserField* field = new LaserField(1000.0, 10.0, 1.0, 2.0, 0.0, 0.0, 
					   ThreeVector(0,0,0), ThreeVector(0,0,1));

	// Set up the primary and secondary particle lists
	ParticleList* electrons = new ParticleList();
	ParticleList* positrons = new ParticleList();
	ParticleList* gammas	= new ParticleList();
	electrons->GenericSource(1, 1, 1, 1, 0, ThreeVector(0,0,-1), ThreeVector(0,0,1));

	// Set the particle pusher
	ParticlePusher* pusher = new ParticlePusher(field, 0.001);

	// Set the physical processes 
	NonLinearCompton* compton = new NonLinearCompton(field, 0.001);

	// Set the output file
	HDF5Output* file = new HDF5Output("./Data/test.h");

	// Enter the main pushing loop
	int n = 0;
	while(true)
	{
		// Push electrons
		for (unsigned int i = 0; i < electrons->GetNPart(); i++)
		{
			pusher->PushParticle(electrons->GetParticle(i));
			compton->Interact(electrons->GetParticle(i), gammas);
		}
		// Push positrons
		for (unsigned int i = 0; i < positrons->GetNPart(); i++)
		{
			pusher->PushParticle(positrons->GetParticle(i));
			compton->Interact(positrons->GetParticle(i), gammas);
		}
		// Push gammas
		for (unsigned int i = 0; i < gammas->GetNPart(); i++)
		{
			pusher->PushParticle(gammas->GetParticle(i));
		}

		// add up optical depth and check if process occurs
		if (n == 10000)
		{
			break;
		}
		n++;
	}

	gammas->SaveTracks(file, "Gamma");
	electrons->SaveTracks(file, "Electron");

	delete field;
	delete electrons;
	delete positrons;
	delete gammas;
	delete compton;
	delete file;

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
