#include <string>
#include <omp.h>

#include "EMField.hh"
#include "GaussianEMField.hh"
#include "StaticEMField.hh"
#include "PlaneEMField.hh"

#include "ParticlePusher.hh"
#include "ParticleList.hh"
#include "SourceGenerator.hh"

#include "NonLinearCompton.hh"
#include "NonLinearBreitWheeler.hh"

#include "FileParser.hh"
#include "Histogram.hh"
#include "OutputManager.hh"

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
	ProcessParameters inProcess = input->GetProcess();
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

	// set up the particle sources
	std::vector<SourceGenerator*> generators(inParticles.size());
	for (unsigned int i = 0; i < inParticles.size(); ++i)
	{
		SourceGenerator* source;
		if (inParticles[i].Distro == "mono")
		{
			source = new SourceGenerator(inParticles[i].Type,
										 inParticles[i].Number,
										 inParticles[i].Energy,
										 inParticles[i].Energy,
										 inParticles[i].Radius,
										 inParticles[i].Position,
										 inParticles[i].Direction);
		} else
		{
			source = new SourceGenerator(inParticles[i].Type,
										 inParticles[i].Number,
				 						 inParticles[i].EnergyMin,
				 						 inParticles[i].EnergyMax,
				 						 inParticles[i].Radius,
				 						 inParticles[i].Position,
				 						 inParticles[i].Direction);
		}
		generators[i] = source;
	}

	// Set up the physics pusher
	ParticlePusher* pusher = new ParticlePusher(field, inGeneral.timeStep);
	
	// Set up the Physics list
	std::vector<Process*> processList;
	if (inProcess.NonLinearCompton == true)
	{
		NonLinearCompton* comptonNL = new NonLinearCompton(field, inGeneral.timeStep);
		processList.push_back(comptonNL);
	} else if (inProcess.NonLinearBreitWheeler == true)
	{
		NonLinearBreitWheeler* breitWheelerNL = new NonLinearBreitWheeler(field, 
													  		inGeneral.timeStep);
		processList.push_back(breitWheelerNL);
	}

	// Set up the histograms
	std::vector<Histogram*> histograms(inHistogram.size());
	for (unsigned int i = 0; i < inHistogram.size(); i++)
	{
		histograms[i] = new Histogram(inHistogram[i].Name, inHistogram[i].Particle,
									  inHistogram[i].Type, inHistogram[i].Time, 
									  inHistogram[i].MinBin, inHistogram[i].MaxBin, 
									  inHistogram[i].Bins);
	}

	unsigned int nEvents(0);
	for (unsigned int i = 0; i < inParticles.size(); i++)
	{
		nEvents += inParticles[i].Number;
	}
	std::cout << "Setup complete! " << nEvents << " events will be simulated.\n";
	std::cout << "Entering main loop using " << omp_get_max_threads();
	std::cout << " threads.\n";
	double startTime = omp_get_wtime();

	// enter main loop
	for (unsigned int i = 0; i < generators.size(); i++) // Loop sources
	{
		#pragma omp parallel for
		for (unsigned int j = 0; j < generators[i]->GetSourceNumber(); j++) // loop events
		{
			// Generate source
			ParticleList* event = generators[i]->GenerateList();

			unsigned int histCount(0);
			double time(0);	
			while(time < inGeneral.timeEnd) // loop time
			{
				// Check if time for histogram
				if (histCount < histograms.size() && time >= histograms[histCount]->GetTime())
				{
					for (unsigned int k = 0; k < event->GetNPart(); k++)
					{
						if (histograms[histCount]->GetParticle() == 
							event->GetParticle(k)->GetName())
						{
							histograms[histCount]->AppParticle(event->GetParticle(k));
						}
					}
					histCount++;
				}

				// Push particles and interact
				for (unsigned int k = 0; k < event->GetNPart(); k++) // Loop particles
				{
					pusher->PushParticle(event->GetParticle(k));
					for (unsigned int proc = 0; proc < processList.size(); proc++) // loop processes
					{
						processList[proc]->Interact(event->GetParticle(k), event);
					}
				}
				time += inGeneral.timeStep;
			}

			// fill any non filled histograms
			for (unsigned int k = histCount; k < histograms.size(); k++)
			{
				for (unsigned int l = 0; l < event->GetNPart(); l++)
				{
					if (histograms[k]->GetParticle() == 
						event->GetParticle(l)->GetName())
					{
						histograms[k]->AppParticle(event->GetParticle(l));
					}
				}
			}
			// Free up the sapce
			generators[i]->FreeSources(event);
		}
	}

	std::cout << "Simulation complete in time: "; 
	std::cout << omp_get_wtime() - startTime << std::endl;
	std::cout << "Saving data to file: " << inGeneral.fileName;
	std::cout << " and cleaning up...\n";
	
	OutputManager* out = new OutputManager(inGeneral.fileName);
	for (unsigned int i = 0; i < inHistogram.size(); i++)
	{
		out->OutputHist(histograms[i]);
		delete histograms[i];
	}
	delete field;
	delete pusher;
	delete input;

	return 0;
}
