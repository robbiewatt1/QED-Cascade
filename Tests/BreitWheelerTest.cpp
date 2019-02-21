
#include "NonLinearBreitWheeler.hh"
#include "PlaneEMField.hh" 
#include "Photon.hh"
#include "ParticleList.hh"
#include "ThreeVector.hh"

int main(int argc, char* argv[])
{
	PlaneEMField* field = new PlaneEMField(1, 1, 0, ThreeVector(0,0,1));
	NonLinearBreitWheeler* test = new NonLinearBreitWheeler(field, 0.1);
	Photon* gamma = new Photon(1000, ThreeVector(0,0,0), ThreeVector(0,0,-1));
	ParticleList* list = new ParticleList("List");
	test->Interact(gamma, list);

	return 0;
}
