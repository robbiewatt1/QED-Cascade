#include <string>
#include "GaussianEMField.hh"
#include "StaticEMField.hh"
#include "PlaneEMField.hh"
#include "ParticlePusher.hh"
#include "ParticleList.hh"
#include "NonLinearCompton.hh"
#include "FileParser.hh"
#include "EMField.hh"
#include "Histogram.hh"
#include "OutputManager.hh"
#include <ctime>

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
	EMField* field;
	if (inField.Type == "static")
	{
		field = new StaticEMField(inField.E, inField.B);
	} else if (inField.Type == "plane")
	{
		field = new PlaneEMField(inField.MaxE, inField.Wavelength, inField.Polerisation,
							   inField.Direction);
	} else
	{
		field = new GaussianEMField(inField.MaxE, inField.Wavelength, inField.Duration,
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
		sources[i] = new ParticleList(inParticles[i].Name);
		if (inParticles[i].Distro == "mono")
		{
			sources[i]->MonoSource(inParticles[i].Number, mass, charge, inParticles[i].Energy,
								   inParticles[i].Radius, inParticles[i].Position,
								   inParticles[i].Direction);
		} else if (inParticles[i].Distro == "linear")
		{
			sources[i]->LinearSource(inParticles[i].Number, mass, charge,
									 inParticles[i].EnergyMin, inParticles[i].EnergyMax,
									 inParticles[i].Radius, inParticles[i].Position,
									 inParticles[i].Direction);
		}
	}
	ParticleList* secondaries = new ParticleList("Secondaries");

	// Set up the physics processes / pusher
	ParticlePusher* pusher = new ParticlePusher(field, inGeneral.timeStep);
	NonLinearCompton* compton = new NonLinearCompton(field, inGeneral.timeStep);

	// Set up the histograms
	std::vector<Histogram*> histograms(inHistogram.size());
	for (unsigned int i = 0; i < inHistogram.size(); i++)
	{
		histograms[i] = new Histogram(inHistogram[i].Name, inHistogram[i].Particle,
									  inHistogram[i].Type, inHistogram[i].Time, 
									  inHistogram[i].MinBin, inHistogram[i].MaxBin, 
									  inHistogram[i].Bins);
	}

	// Add particle list to file
	OutputManager* out = new OutputManager(inGeneral.fileName);
	out->ListProperties(sources[0], "t0");

	// main loop
	unsigned int histCount(0);
	unsigned int stepCount(0);
	double time(0);
	double start = std::clock();
	while(time < inGeneral.timeEnd)
	{
		std::cout << "At step: " << stepCount << ":" << std::endl;
		std::cout << "Current time: " << time << " / " << inGeneral.timeEnd << std::endl;
		std::cout << "Number of particles: " << secondaries->GetNPart() << std::endl;
		std::cout << "\n";
		// Check if time for histogram
		if (histCount < histograms.size() && time >= histograms[histCount]->GetTime())
		{
			for (unsigned int i = 0; i < sources.size(); i++)
			{
				if (histograms[histCount]->GetParticle() == sources[i]->GetName())
				{
					std::cout << "Filling histogram!" << std::endl;
					histograms[histCount]->Fill(sources[i]);
				}
			}
			histCount++;
		}

		for (unsigned int i = 0; i < sources.size(); i++)
		{
			pusher->PushParticleList(sources[i]);
			for (unsigned int j = 0; j < sources[i]->GetNPart(); ++j)
			{
				compton->Interact(sources[i]->GetParticle(j), secondaries);
			}
		}
		stepCount++;
		time += inGeneral.timeStep;
	}

    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout << "Time taken: "<< duration <<"s\n";
	out->ListProperties(sources[0], "t1");


	// Output to file
	for (unsigned int i = 0; i < histograms.size(); i++)
	{
		out->OutputHist(histograms[i]);
	}

	return 0;
}