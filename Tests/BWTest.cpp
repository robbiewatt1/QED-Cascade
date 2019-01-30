#include "ThermalBW.hh"

int main(int argc, char* argv[])
{
	ThermalRadField* field = new ThermalRadField(1);
	ThermalBW* breitWheeler = new ThermalBW(field, 0.1, 2000, "test", true);
	return 0;
}

