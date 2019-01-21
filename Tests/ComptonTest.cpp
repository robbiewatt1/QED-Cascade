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
#include <iostream>
#include <string>
#include <cstring>
#include "INIReader.hh"
#include "UnitsSystem.hh"
#include "StaticField.hh"
#include "OutputManager.hh"

int main(int argc, char* argv[])
{
	// main script used in development and testing of NonLinearCompton class
	UnitsSystem* units = new UnitsSystem();
        units->SetReferenceFrequencySI(1);

	ThreeVector start = ThreeVector(0, 0, 5);
	ThreeVector focus = ThreeVector(0, 0, 0);
	LaserField* field = new LaserField(0.4,           // Max Efield
                                           1.0,         // Wavelength
                                           5,           // duration
                                           2,           // waist
                                           0,           // pol angle
                                           start,       // start location
                                           focus);      // focus point	);
	NonLinearCompton* compton = new NonLinearCompton(field, units, 0.01);
	ParticleList* gammas = new ParticleList();
	ParticlePusher* pusher = new ParticlePusher(field, 0.01);


	ThreeVector x = ThreeVector(0,0,0);
 	ThreeVector p = ThreeVector(0,0,2000);
        Particle part = Particle(1.0, 1.0, 0, false);
	part.UpdateTrack(x,p);
        
	/*
	std::ofstream outf("chi-test.dat");
	for(int i =0; i < 1e6; i++)
	{
		outf << compton->CalculateChi(0.1) << "\n";
	}
	*/

	for(int i=0; i < 1000; i++)
	{
		// Push particle
		pusher->PushParticle(part);
		// Push Photons
		pusher->PushParticleList(gammas);
		compton->Interact(part, gammas);
	}

	// Output
	HDF5Output* file = new HDF5Output("Copton.h5");
	OutputManager* outMan = new OutputManager(file);
	Histogram* hist = new Histogram("PhotonEnergy",0,100,30);
	hist->Fill(gammas, "energy");
	
	outMan->OutputHist(hist);
//	outMan->SingleParticle(part,"Electron");


	return 0;
}
