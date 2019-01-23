#include "UnitsSystem.hh"


UnitsSystem::UnitsSystem(std::string units)
{
	if (units == "SI" || units == "si")
	{
		SetUnitsSI();
	} else 
	{
		std::cerr << "Units error: Unknowen system \"" << units << "\" given.\n";
		std::cerr << "Exiting!\n";
		exit(1);
	}
}

UnitsSystem::~UnitsSystem()
{
}

void UnitsSystem::SetUnitsSI()
{
	// dimension refrence values
	ref_freq     = SI_eSchw * SI_qe / (SI_me * SI_c);
	ref_time     = 1.0 / ref_freq;
	ref_length   = SI_c / ref_freq;
	ref_eField   = SI_eSchw;
	ref_bField   = SI_eSchw / SI_c;
	ref_momentum = SI_me * SI_c;
	ref_energy   = SI_me * SI_c * SI_c;

	// Constants in terms of refrence
//	lComp = SI_lComp / ref_length;
//	hBar  = SI_hBar / (SI_me * SI_c * SI_c * ref_time);
//	eCrit = SI_me * SI_me * SI_c * SI_c * SI_c / (SI_qe * SI_hBar) / ref_eField;
//	eps0  = SI_eps0  * (SI_me * SI_c * SI_c) * ref_length / (SI_qe * SI_qe);
}
