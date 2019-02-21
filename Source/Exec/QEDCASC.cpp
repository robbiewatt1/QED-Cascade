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
	std::vector<ParticleList*> eventList;
	for (unsigned int i = 0; i < inParticles.size(); i++)
	{
	//	sources[i] = new ParticleList(inParticles[i].Name);
		if (inParticles[i].Distro == "mono")
		{
			std::vector<ParticleList*> source = SourceGenerator::MonoSource(inParticles[i].Type, 
														inParticles[i].Number, inParticles[i].Energy, 
														inParticles[i].Radius, inParticles[i].Position, 
														inParticles[i].Direction);
			eventList.insert(eventList.end(), source.begin(), source.end());
		} else if (inParticles[i].Distro == "linear")
		{
			std::vector<ParticleList*> source = SourceGenerator::LinearSource(inParticles[i].Type,
														inParticles[i].Number, inParticles[i].EnergyMin, 
														inParticles[i].EnergyMax, inParticles[i].Radius, 
														inParticles[i].Position, inParticles[i].Direction);
			eventList.insert(eventList.end(), source.begin(), source.end());
		}
	}

	// Set up the physics processes / pusher
	ParticlePusher* pusher = new ParticlePusher(field, inGeneral.timeStep);
	NonLinearCompton* compton = new NonLinearCompton(field, inGeneral.timeStep);
	NonLinearBreitWheeler* breitWheeler = new NonLinearBreitWheeler(field, 
													  inGeneral.timeStep);
	// Set up the histograms
	std::vector<Histogram*> histograms(inHistogram.size());
	for (unsigned int i = 0; i < inHistogram.size(); i++)
	{
		histograms[i] = new Histogram(inHistogram[i].Name, inHistogram[i].Particle,
									  inHistogram[i].Type, inHistogram[i].Time, 
									  inHistogram[i].MinBin, inHistogram[i].MaxBin, 
									  inHistogram[i].Bins);
	}

	// main loop
	std::cout << "Setup complete! " << eventList.size() << " events will be simulated.\n";
	std::cout << "Entering main loop using " << omp_get_max_threads();
	std::cout << " threads.\n";
	double startTime = omp_get_wtime();

	#pragma omp parallel for
	for (unsigned int i = 0; i < eventList.size(); i++)
	{
		unsigned int histCount(0);
		double time(0);	
		while(time < inGeneral.timeEnd)
		{
			// Check if time for histogram
			if (histCount < histograms.size() && time >= histograms[histCount]->GetTime())
			{
				for (unsigned int j = 0; j < eventList[i]->GetNPart(); j++)
				{
					if (histograms[histCount]->GetParticle() == 
						eventList[i]->GetParticle(j)->GetName())
					{
						histograms[histCount]->AppParticle(eventList[i]->GetParticle(j));
					}
				}
				histCount++;
			}

			// Push particles and interact
			for (unsigned int j = 0; j < eventList[i]->GetNPart(); j++)
			{
				pusher->PushParticle(eventList[i]->GetParticle(j));
				compton->Interact(eventList[i]->GetParticle(j), eventList[i]);
				breitWheeler->Interact(eventList[i]->GetParticle(j), eventList[i]);
			}
			time += inGeneral.timeStep;
		}

		// fill any non filled histograms
		for (unsigned int j = histCount; j < histograms.size(); j++)
		{
			for (unsigned int k = 0; k < eventList[i]->GetNPart(); k++)
			{
				if (histograms[j]->GetParticle() == 
					eventList[i]->GetParticle(k)->GetName())
				{
					histograms[j]->AppParticle(eventList[i]->GetParticle(k));
				}
			}
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
	SourceGenerator::FreeSources(eventList);
	delete field;
	delete compton;
	delete pusher;
	delete input;

	return 0;
}
