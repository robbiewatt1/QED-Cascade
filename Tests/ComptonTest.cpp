#include "GaussianEMField.hh"
#include "Particle.hh"
#include "PlaneEMField.hh"
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
#include "StaticEMField.hh"
#include "OutputManager.hh"

int main(int argc, char* argv[])
{
	// main script used in development and testing of NonLinearCompton class
/*
	ThreeVector start = ThreeVector(0, 0, 5);
	ThreeVector focus = ThreeVector(0, 0, 0);
	LaserField* field2 = new LaserField(0.01,       // Max Efield
                                           1.0,         // Wavelength
                                           5,           // duration
                                           2,           // waist
                                           0,           // pol angle
                                           start,       // start location
                                           focus);      // focus point	);
        PlaneField* field = new PlaneField(6e-5,	// Max Field
                                           2.3e6,	// Wave Length
                                           0.0,		// pol angle
                                           ThreeVector(0,0,-1));	// Direction

	NonLinearCompton* compton = new NonLinearCompton(field, 5e2);

	ParticleList* gammas = new ParticleList();
	ParticleList* electrons = new ParticleList();
	electrons->GenericSource(200, 		// n particles
				 1,		//  mass
				 -1,		// charge
				 4000,		// energy
				 2.3e6, 	// spot size
				 ThreeVector(0,0,0),	// pos
				 ThreeVector(0,0,1));	// direction

	ParticlePusher* pusher = new ParticlePusher(field, 5e2);

        HDF5Output* file = new HDF5Output("Compton.h5");
        OutputManager* outMan = new OutputManager(file);

        Histogram* hist1 = new Histogram("ElectronEnergyB",500,5000,50);
        hist1->Fill(electrons, "energy");
        

	std::ofstream outf("chi-test.dat");
	for(int i =0; i < 1e6; i++)
	{
		outf << compton->CalculateChi(0.1) << "\n";
	}

	for(int i=0; i < 20000; i++)
	{
		// Push particle
		pusher->PushParticleList(electrons);
		// Push Photons
//		pusher->PushParticleList(gammas);
		for(int j = 0; j < electrons->GetNPart(); j++)
		{
			compton->Interact(electrons->GetParticle(j), gammas);
		}
	}

	// Output
	Histogram* hist2 = new Histogram("ElectronEnergyA",500,5000,50);
	hist2->Fill(electrons, "energy");
	
	outMan->OutputHist(hist1);
	outMan->OutputHist(hist2);
//	outMan->SingleParticle(part,"Electron");

*/
	return 0;
}
