#include "LaserField.hh"
#include "PlaneField.hh"
#include "ThreeVector.hh"
#include "HDF5Output.hh"
#include "OutputManager.hh"
#include <vector>


int main(int argc, char* argv[])
{
	ThreeVector start = ThreeVector(0,0,6);
	ThreeVector focus = ThreeVector(0,-1,-1);
	LaserField* field = new LaserField(1, 		// Max intensity
					   0.5,		// Wavelength
					   3,		// duration
					   2,		// waist
					   0,		// pol angle
					   start,	// start location
					   focus);	// focus point

	PlaneField* field2 = new PlaneField(1,
					    3,
					    0.0,
					    ThreeVector(0,0,1));

        HDF5Output* file = new HDF5Output("Field.h5");
        OutputManager* outMan = new OutputManager(file);

        std::vector<double> zax(500);
        for (int i = 0; i < 500; ++i)
        {
                zax[i] = -6.0 + (double)i / 40;
        }
        std::vector<double> xax(3);
        for (int i = 0; i < 3; ++i)
        {
                xax[i] = -5.0 + (double)i / 0.3;
        }
        std::vector<double> time(12);
        for(int i = 0; i < 12; i++)
        {
                time[i] = (double) i;
        }

	outMan->EMField(field2, time, xax, zax, zax);




	return 0;
}
