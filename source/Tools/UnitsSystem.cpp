#include "UnitsSystem.hh"


UnitsSystem::UnitsSystem()
{
}

void UnitsSystem::SetReferenceFrequencySI(double frequency)
{
	// dimension refrence values
	ref_freq   = frequency;
	ref_time   = 1.0 / frequency;
	ref_length = SI_c / frequency;
	ref_eField = SI_me * SI_c * frequency / SI_qe;
	ref_bField = SI_me * frequency / SI_qe;

	// Constants in terms of refrence
	lComp = SI_lComp / ref_length;
	hBar  = SI_hBar / (SI_me * SI_c * SI_c * ref_time);
	eCrit = SI_me * SI_me * SI_c * SI_c * SI_c / (SI_qe * SI_hBar) / ref_eField;
	eps0  = SI_eps0  * (SI_me * SI_c * SI_c) * ref_length / (SI_qe * SI_qe);
}
