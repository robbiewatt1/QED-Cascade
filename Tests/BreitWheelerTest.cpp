
#include "NonLinearBreitWheeler.hh"
#include "PlaneEMField.hh" 

int main(int argc, char* argv[])
{
	PlaneEMField* field = new PlaneEMField(1, 1, 0, ThreeVector(0,0,1));
	NonLinearBreitWheeler* test = new NonLinearBreitWheeler(field, 1);

//	Particle gamma = Particle(

	return 0;
}
