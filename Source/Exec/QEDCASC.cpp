#include <string>

#include "EMField.hh"
#include "GaussianEMField.hh"
#include "StaticEMField.hh"
#include "PlaneEMField.hh"
#include "FocusingField.hh"

#include "ParticlePusher.hh"
#include "LorentzPusher.hh"
#include "LandauPusher.hh"
#include "ModifiedLandauPusher.hh"

#include "ParticleList.hh"
#include "SourceGenerator.hh"

#include "ContinuousEmission.hh"
#include "StochasticEmission.hh"
#include "NonLinearBreitWheeler.hh"

#include "FileParser.hh"
#include "Histogram.hh"
#include "OutputManager.hh"

#include "MCTools.hh"

#ifdef USEOPENMP
    #include <omp.h>
#endif
#ifdef USEMPI
    #include <mpi.h>
#endif

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

    // set up MPI if we are using it 
#ifdef USEMPI
    MPI_Init(&argc, &argv);
    // Set random seed with MPI rank
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MCTools::SetSeed(id);
#endif
    // Parse the file
    FileParser* input = new FileParser(argv[1], true);
    // Get the input paramter structs
    GeneralParameters inGeneral = input->GetGeneral();
    FieldParameters inField = input->GetField();
    PhysicsParameters inPhysics = input->GetPhysics();
    std::vector<ParticleParameters> inParticles = input->GetParticle();
    std::vector<HistogramParameters> inHistogram = input->GetHistograms();
    delete input;


    // Set up the fields
    EMField* field;
    if (inField.Type == "static")
    {
        field = new StaticEMField(inField.E, inField.B);
    } else if (inField.Type == "plane")
    {
        field = new PlaneEMField(inField.MaxE, inField.Wavelength,
            inField.Polerisation, inField.Direction);
    } else if (inField.Type == "gaussian")
    {
        field = new GaussianEMField(inField.MaxE, inField.Wavelength,
            inField.Duration, inField.Waist, inField.Polerisation,
            inField.Start, inField.Focus);
    } else if (inField.Type == "focusing")
    {
        field = new FocusingField(inField.MaxE, inField.Wavelength,
            inField.Duration, inField.Waist, inField.Polerisation,
            inField.Start, inField.Focus);
    } else
    {
        std::cerr << "Error: unknown field type." << std::endl;
        return 1;
    }

    // Set the physics
    ParticlePusher* pusher;
    std::vector<Process*> processList;
    if (inPhysics.Physics == "Classical" || inPhysics.Physics == "classical")
    {
        pusher = new LandauPusher(field, inGeneral.timeStep);
        ContinuousEmission* emission = new ContinuousEmission(field,
            inGeneral.timeStep, true, inPhysics.SampleFraction,
            inPhysics.MinEnergy, inGeneral.tracking);
        processList.push_back(emission);
    } else if (inPhysics.Physics == "Semiclassical" || inPhysics.Physics == "semiclassical")
    {
        pusher = new ModifiedLandauPusher(field, inGeneral.timeStep);
        ContinuousEmission* emission = new ContinuousEmission(field,
            inGeneral.timeStep, false, inPhysics.SampleFraction,
            inPhysics.MinEnergy, inGeneral.tracking);
        processList.push_back(emission);
    } else if (inPhysics.Physics == "Quantum" || inPhysics.Physics == "quantum")
    {
        pusher = new LorentzPusher(field, inGeneral.timeStep);
        StochasticEmission* emission = new StochasticEmission(field,
            inGeneral.timeStep, inPhysics.SampleFraction, inPhysics.MinEnergy,
            inGeneral.tracking);
        processList.push_back(emission);
    } else
    {
        std::cerr << "Error: Unknown physics module \"" << inPhysics.Physics 
                  << "\"" << std::endl;
        return 1; 
    }

    if (inPhysics.PairProduction == true)
    {
        NonLinearBreitWheeler* breitWheeler = new NonLinearBreitWheeler(field, 
            inGeneral.timeStep, false);
        processList.push_back(breitWheeler);
    }

    // set up the particle sources
    std::vector<SourceGenerator*> generators(inParticles.size());
    for (unsigned int i = 0; i < inParticles.size(); ++i)
    {
        SourceGenerator* source = new SourceGenerator(inParticles[i].Type,
                inParticles[i].Distro, inParticles[i].Number,
                inParticles[i].Energy1, inParticles[i].Energy2,
                inParticles[i].Radius, inParticles[i].Duration,
                inParticles[i].Divergence, inParticles[i].Position, 
                inParticles[i].Direction, inGeneral.tracking);
        generators[i] = source;
    }

    // Set up the histograms
    std::vector<Histogram*> histograms(inHistogram.size());
    for (unsigned int i = 0; i < inHistogram.size(); i++)
    {
        histograms[i] = new Histogram(inHistogram[i].Name,
            inHistogram[i].Particle, inHistogram[i].Type, inHistogram[i].Time,
            inHistogram[i].MinBin, inHistogram[i].MaxBin, inHistogram[i].Bins);
    }

    // Set up output manager
    OutputManager* out = new OutputManager(inGeneral.fileName);
    // Set up is complete, print info
    unsigned int nEvents(0);
    for (unsigned int i = 0; i < inParticles.size(); i++)
    {
        nEvents += inParticles[i].Number;
    }

#ifdef USEOPENMP
    std::cout.precision(2);
    std::cout << "Setup complete! " << nEvents << " events will be simulated.\n";
    std::cout << "Entering main loop using " << omp_get_max_threads();
    std::cout << " threads.\n";
    double startTime = omp_get_wtime();
#endif
    // enter main loop
    for (unsigned int i = 0; i < generators.size(); i++) // Loop sources
    {
        // set up the full event store
        if (inParticles[i].Output == true) out->InitSource(generators[i]->GetSourceNumber());

#ifdef USEOPENMP
        int threadEvents = generators[i]->GetSourceNumber();
        #pragma omp parallel for
#endif
        for (unsigned int j = 0; j < generators[i]->GetSourceNumber(); j++) // loop events
        {
            // Generate source
            ParticleList* event = generators[i]->GenerateList();

            // Store full event info
            if (inParticles[i].Output == true) out->StoreSource(event, j, true);

            unsigned int histCount(0);
            double time(0);
            while(time < inGeneral.timeEnd) // loop time
            {
                // Check if time for histogram
                if (histCount < histograms.size())
                {
                 //   std::cout << histCount << " " << histograms[histCount]->GetTime() << std::endl;
                    if(time >= histograms[histCount]->GetTime())
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

            // Store source data and tracking
            if (inParticles[i].Output == true) out->StoreSource(event, j, false);
            if (inGeneral.tracking == true) out->StoreTrack(event, j);

            // Free up the sapce
            generators[i]->FreeSources(event);
            
            // Approx % complete timer
#ifdef USEOPENMP

            if (omp_get_thread_num() == 0 && j % 5 == 0)
            {
                std::cout << "Approximately " << 
                (double) (j* omp_get_max_threads()) / threadEvents * 100.0 
                << "% complete \r";
            }
#endif
        }
#ifdef USEMPI
        out->OutputEventsMPI(inParticles[i].Output, inGeneral.tracking);
#else
        out->OutputEvents(inParticles[i].Output, inGeneral.tracking);
#endif
    }

#ifdef USEOPENMP
    std::cout << "Simulation complete in time: "; 
    std::cout << omp_get_wtime() - startTime << " s" << std::endl;
    std::cout << "Saving data to file: " << inGeneral.fileName;
    std::cout << " and cleaning up...\n";
#endif

    for (unsigned int i = 0; i < inHistogram.size(); i++)
    {
#ifdef USEMPI
        out->OutputHistMPI(histograms[i]);
#else
        out->OutputHist(histograms[i]);
#endif
        delete histograms[i];
    }

    delete field;
    delete pusher;
    delete out;

#ifdef USEMPI  
    MPI_Finalize();
#endif

    return 0;
}
