#include "Particle.hh"
#include "ThreeVector.hh"
#include "ParticlePusher.hh"
#include "ThreeMatrix.hh"
#include "ParticleList.hh"
#include "MCTools.hh"
#include <fstream>
#include <string>
#include <cstring>
#include "UnitsSystem.hh"

#include "HDF5Output.hh"
#include "OutputManager.hh"

#include "StaticField.hh"

int main(int argc, char* argv[])
{
	/*
	HDF5Output* file = new HDF5Output("./test.h5");
	OutputManager* outManager = new OutputManager(file);
	
	StaticField* field = new StaticField(ThreeVector(0.005,0,0), ThreeVector(0,1,0));
	
	ParticlePusher* pusher = new ParticlePusher(field, 0.01);
	
	ThreeVector x = ThreeVector(0,0,0);
	ThreeVector p = ThreeVector(0.1,0.1,0.1);

	Particle part = Particle(1.0, 1.0, 0, true);
	part.UpdateTrack(x,p);

	for(int i = 0; i < 10000; i++)
	{
		pusher->PushParticle(part);
	}
	
	outManager->SingleParticle(part, "Electron");

	*/	
	return 0;
}
