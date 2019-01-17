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


int main(int argc, char* argv[])
{
	// main script used in development and testing of NonLinearCompton class
	UnitsSystem* units = new UnitsSystem();
        units->SetReferenceFrequencySI(1);
        StaticField* field = new StaticField(ThreeVector(1,0,0),ThreeVector(0,-1,0));
	NonLinearCompton* compton = new NonLinearCompton(field, units, 1.0);
	ParticleList* partList = new ParticleList();

	ThreeVector x = ThreeVector(0,0,0);
 	ThreeVector p = ThreeVector(0,0,10);

        Particle part = Particle(1.0, 1.0, 0, false);
	part.UpdateTrack(x,p);
        
	std::ofstream outf("chi-test.dat");
	for(int i =0; i < 1e6; i++)
	{
		outf << compton->CalculateChi(0.1) << "\n";
	}
	
	return 0;
}
