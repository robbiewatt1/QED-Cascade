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
#include <string>
#include <cstring>
#include "INIReader.hh"
#include "UnitsSystem.hh"


int main(int argc, char* argv[])
{
	// main script used in development and testing of NonLinearCompton class
	UnitsSystem* units = new UnitsSystem();
        units->SetReferenceFrequencySI(1);
        LaserField* field = new LaserField(10.0, 2.0, 1.0, 1.0, 0.0,
                                           0.0, ThreeVector(0,0,1),ThreeVector(0,0,0));
	NonLinearCompton* compton = new NonLinearCompton(field, units, 0.1);
	return 0;
}
