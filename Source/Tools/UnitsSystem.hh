#ifndef UNITSSYSTEM_HH
#define UNITSSYSTEM_HH

#include <string>
#include "ThreeVector.hh"

class UnitsSystem
{
public:
	/* 
	This class is used for setting the units in the simulation. The main particle tracker 
	algorythm is all defined in dimensionless units with an unkownen refrence frequency 
	and so is not required. The reference frequency is required for QED interaction
	probabilities and requires the user to define a reference frequency in physical units e.g
	SI. This class is also used to convert all units back at the end.
	*/

	UnitsSystem(std::string system);

	~UnitsSystem();
	
	// Method to get refrence values and physical constants
	double RefFrequency() const {return ref_freq;} 

	double RefTime() const {return ref_time;}

	double RefLength() const {return ref_length;}

	double RefEField() const {return ref_eField;}

	double RefBField() const {return ref_bField;}

	double RefMomentum() const {return ref_momentum;}

	double RefEnergy() const {return ref_energy;}

	// Methods to geth g4 units
	double G4Time() const {return g4_time;}

	double G4Length() const {return g4_length;}

	double G4Energy() const {return g4_energy;}

private:

	void SetUnitsSI();

	void SetG4Units();

private:

	// List of refrence values
	double ref_freq;
	double ref_time;
	double ref_length;
	double ref_eField;
	double ref_bField;
	double ref_momentum;
	double ref_energy;

	// Physical constant in terms of refrence values
	double hBar;	// reduce planks constant
	double eCrit;	// Schwinger field
	double lComp;	// Compton wavelength
	double eps0;	// electric constant

	// Constants for transforming to Geant units
	double g4_time;
	double g4_length;
	double g4_energy;


public:
	// dimensionless constants
	static constexpr double pi 	  = 3.14159265359;
	static constexpr double e  	  = 2.71828182845;
	static constexpr double alpha = 0.0072973525664;

	// Consatnts in S.I
	static constexpr double SI_c 	 = 2.99792458e8;
	static constexpr double SI_me 	 = 9.10938356e-31;
	static constexpr double SI_qe 	 = 1.60217662e-19;
	static constexpr double SI_hBar	 = 1.05457180e-34;
	static constexpr double SI_lComp = 2.4263102367e-12;
	static constexpr double SI_eps0	 = 8.854187817e-12;
	static constexpr double SI_eSchw = 1.3232855e18;

};
#endif