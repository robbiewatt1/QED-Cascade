#include "LaserField.hh"
#include "StaticField.hh"
#include "PlaneField.hh"
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
#include "FileParser.hh"
#include "Field.hh"
#include "Histogram.hh"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cerr << "Error: Input file was not provided\n";
		std::cerr << "Please provide an input file via the command line.\n";
		std::cerr << "For help on using \"QED-Cascade\", and for a full list of command line " 
					 "options, please provide the command line argument \"-h\".\n";
		return 1;
	} else if (argc > 2)
	{
		std::cerr << "Error: " << argc << " command line arguments provided\n";
		std::cerr << "\"QED-Cascade\" only accepts 1 command line argument\n";
		std::cerr << "For help on using \"QED-Cascade\", and for a full list of command line" 
					 " options, please provide the command line argument \"-h\".\n";
		return 1;
	} else
	{
		std::string argument(argv[1]);
		if (argument == "-h")
		{
			std::cout << "This is how to use the code\n";
			return 1;
		} else if (argument.substr(argument.size() - 4) == ".ini")
		{

		} else
		{
			std::cerr << "Error: unrecognised command line argument \"" << argv[1] << "\" provided.\n";
			std::cerr << "For help on using \"QED-Cascade\", and for a full list of command line "
					 	 "options, please provide the command line argument \"-h\".\n";
			return 1;
		}
	}

	// Parse the file
	FileParser* input = new FileParser(argv[1]);

	// Get the input paramter strucs
	GeneralParameters inGeneral = input->GetGeneral();
	FieldParameters inField = input->GetField();
	std::vector<ParticleParameters> inParticles = input->GetParticle();
	std::vector<HistogramParameters> inHistogram = input->GetHistograms();

	// Set up the fields
	Field* field;
	if (inField.Type == "static")
	{
		field = new StaticField(inField.E, inField.B);
	} else if (inField.Type == "plane")
	{
		field = new PlaneField(inField.MaxE, inField.Wavelength, inField.Polerisation,
							   inField.Direction);
	} else
	{
		field = new LaserField(inField.MaxE, inField.Wavelength, inField.Duration,
							   inField.Waist, inField.Polerisation, inField.Start,
							   inField.Focus);
	}

	// Set up the particle lists
	std::vector<ParticleList*> sources(inParticles.size());
	for (unsigned int i = 0; i < inParticles.size(); i++)
	{

		double mass, charge;
		if (inParticles[i].Type == "electron")
		{
			mass = 1.0;
			charge = -1.0;
		} else if (inParticles[i].Type == "positron")
		{
			mass = 1.0;
			charge = 1.0;
		} else
		{
			mass = 0;
			charge = 0;
		}
		sources[i] = new ParticleList();
	
		sources[i]->GenericSource(inParticles[i].Number, mass, charge, inParticles[i].Energy,
								  inParticles[i].Radius, inParticles[i].Position,
								  inParticles[i].Direction);
	}
	ParticleList* secondaries = new ParticleList();

	// Set up the physics processes / pusher
	ParticlePusher* pusher = new ParticlePusher(field, inGeneral.timeStep);
	NonLinearCompton* compton = new NonLinearCompton(field, inGeneral.timeStep);

	std::cout << "here" << std::endl;
	// Set up the histograms
	std::vector<Histogram*> histograms(inHistogram.size());
	for (unsigned int i = 0; i < inHistogram.size(); i++)
	{
		histograms[i] = new Histogram(inHistogram[i].Name, inHistogram[i].Type,
									  inHistogram[i].Time, inHistogram[i].MinBin,
									  inHistogram[i].MaxBin, inHistogram[i].Bins);
	}

	// main loop
	double time(0);
	while(time < inGeneral.timeEnd)
	{
		std::cout << time << std::endl;
		for (unsigned int i = 0; i < sources.size(); i++)
		{
			pusher->PushParticleList(sources[i]);
			for (unsigned int j = 0; j < sources[i]->GetNPart(); ++j)
			{
				//compton->Interact
			}
			
		}
	}



/*
	// General
	double timeStep 	= inputFile.GetReal("General", "time_step", 0);
	double timeEnd 		= inputFile.GetReal("General", "time_end", 0);
	double refFrequency = inputFile.GetReal("General", "frequency_ref_SI", 0);
	// Laser
	double l_intensity 		= inputFile.GetReal("Laser", "intensity", 0);
	double l_duration 		= inputFile.GetReal("Laser", "duration", 0);
	double l_waleLength 	= inputFile.GetReal("Laser", "waleLength", 0);
	double l_waist			= inputFile.GetReal("Laser", "waist", 0);
	double l_polarisation	= inputFile.GetReal("Laser", "polarisation", 0);
	ThreeVector l_location 	= inputFile.GetThreeVector("Laser", "location", ThreeVector(0,0,0));
	l_location.Print();
	ThreeVector l_direction = inputFile.GetThreeVector("Laser", "direction", ThreeVector(0,0,1));
	l_direction.Print();
	// Electrons
	int e_number = inputFile.GetInteger("Electrons", "number", 0);
	int e_energy = inputFile.GetInteger("Electrons", "energy", 1);
	int e_deltaR = inputFile.GetInteger("Electrons", "delta_r", 0);
	ThreeVector e_location 	= inputFile.GetThreeVector("Electrons", "location", ThreeVector(0,0,0));
	e_location.Print();
	ThreeVector e_direction = inputFile.GetThreeVector("Electrons", "direction", ThreeVector(0,0,1));
	e_direction.Print();
	// Positrons
	// Gammas
	// Output

/*
	// Set up the simulation
	UnitsSystem* units = new UnitsSystem();
	units->SetReferenceFrequencySI(1);
	LaserField* field = new LaserField(l_intensity, l_duration, l_waleLength, l_waist, 0.0, 
									   l_polarisation, l_direction, l_location);
	
	ParticlePusher* pusher = new ParticlePusher(field, timeStep);
	
	NonLinearCompton* compton = new NonLinearCompton(field, units, timeStep);
	
	HDF5Output* file = new HDF5Output("./Data/test.h");
	
	ParticleList* electrons = new ParticleList();
	ParticleList* positrons = new ParticleList();
	ParticleList* gammas	= new ParticleList();
	electrons->GenericSource(e_number, 1, -1, e_energy, e_deltaR, e_location, e_direction);


	// Enter the main simulation loop
	int n = 0;
	while(true)
	{
		// Push electrons
		for (unsigned int i = 0; i < electrons->GetNPart(); i++)
		{
			pusher->PushParticle(electrons->GetParticle(i));
			compton->Interact(electrons->GetParticle(i), gammas);
		}
		// Push positrons
		for (unsigned int i = 0; i < positrons->GetNPart(); i++)
		{
			pusher->PushParticle(positrons->GetParticle(i));
			compton->Interact(positrons->GetParticle(i), gammas);
		}
		// Push gammas
		for (unsigned int i = 0; i < gammas->GetNPart(); i++)
		{
			pusher->PushParticle(gammas->GetParticle(i));
		}

		// add up optical depth and check if process occurs
		if (n == 20000)
		{
			break;
		}
		n++;
	}


	gammas->SaveTracks(file, "Gamma");
	electrons->SaveTracks(file, "Electron");

	delete field;
	delete electrons;
	delete positrons;
	delete gammas;
	delete compton;
	delete file;
*/
	// define time using laser frequency with 
/*
	Particle electron = Particle(1, -1, true);
	electron.UpdateTrack(ThreeVector(0.0, 0.0, 0.0), ThreeVector(0.0, 0.0, 0.0));

	LaserField* field = new LaserField(100.0, 2.0, 1.0, 2.0, -8.0, 0.0, ThreeVector(0,0,0), ThreeVector(0,0,1));
	ParticlePusher pusher = ParticlePusher(field, 0.001);
	for (int i = 0; i < 100000; ++i)
	{
		pusher.PushParticle(electron);
	}
	HDF5Output file = HDF5Output("./Data/test.h");
	electron.SaveTrack(file);


	std::vector<double> zax(500);
	for (int i = 0; i < 500; ++i)
	{
		zax[i] = -20.0 + (double)i / 12.5;
	}
	std::vector<double> xax(3);
        for (int i = 0; i < 3; ++i)
        {
                xax[i] = -5.0 + (double)i / 0.3;
        }
	std::vector<double> time(20);
	for(int i = 0; i < 20; i++)
	{
		time[i] = (double) i;
	}
	field->SaveField(file, time, zax, zax, zax);
*/
	return 0;
}
