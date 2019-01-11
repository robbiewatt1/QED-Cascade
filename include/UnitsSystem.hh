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

	UnitsSystem();

//	UnitsSystem(double refFrequency, std::string units);

	~UnitsSystem();
	
	void SetReferenceFrequencySI(double frequency);

	// Method to get refrence values and physical constants
	double RefFrequency() const {return ref_freq;} 

	double RefTime() const {return ref_time;}

	double RefLength() const {return ref_length;}

	double RefEField() const {return ref_freq;}

	double RefBField() const {return ref_bField;}

	// Method for getting physical constants in terms of reference values
	double HBar() const {return hBar;}

	double ECrit() const {return eCrit;}

	double LComp() const {return lComp;}

	// Methods to convert SI units to refrence values
	double Freq2Ref(double freq) const {return freq / ref_freq;}

	double Time2Ref(double time) const {return time / ref_time;}

	double Length2Ref(double length) const {return length / ref_length;}

	double EField2Ref(double eField) const {return eField / ref_eField;}

	double BField2Ref(double bField) const {return bField / ref_bField;}

	// Methods to convert values back to SI
	double Freq2SI(double freq) const;

	double Time2SI(double time) const;

	double Length2SI(double length) const;

	double EField2SI(double eField) const;

	double BField2SI(double bField) const;

private:

	// List of refrence values
	double ref_freq;
	double ref_time;
	double ref_length;
	double ref_eField;
	double ref_bField;

	// Physical constant in terms of refrence values
	double hBar;	// reduce planks constant
	double eCrit;	// Schwinger field
	double lComp;	// Compton wavelength
	double eps0;	// electric constant

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
};
#endif