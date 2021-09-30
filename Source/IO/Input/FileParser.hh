#ifndef FileParser_HH
#define FILEPARSER_HH

#include "INIReader.hh"
#include "UnitsSystem.hh"
#include <fstream>

struct GeneralParameters
{
    // General
    std::string units;
    double timeStep;        // time step
    double timeEnd;         // end of simulation
    std::string fileName;   // Output file name
    bool tracking;          // Turns on particle tracking
};

struct FieldParameters
{
    std::string Type;       // type of field
    ThreeVector E;          // electric field
    ThreeVector B;          // magnetic field 
    double MaxE;            // Max E field
    double Wavelength;      // laser wavelength
    double Duration;        // laser duration
    double Waist;           // laser waist
    double Polerisation;    // laser polerisation
    ThreeVector Direction;  // plane field direction
    ThreeVector Start;      // start point of laser pulse
    ThreeVector Focus;      // focus point of laser
};

struct ParticleParameters
{
    unsigned int Number;    // number of particle sources
    std::string Name;       // names of source
    std::string Type;       // particle types
    std::string Distro;     // Energy distrobution
    ThreeVector Position;   // positions
    ThreeVector Direction;  // directions
    double Energy1;         // source min energy
    double Energy2;         // source max energy
    double Radius;          // source size
    double Divergence;      // source divergence
    double Duration;        // source Duration
    bool Output;            // Output individual particle data
};

struct PhysicsParameters
{
    std::string Physics;    // Select radiation physics
    double MinEnergy;       // Min energy of tracked particle
    double SampleFraction;  // Down samples 
    bool PairProduction;    // Turn on nonlinear Breit-Wheeler
};

struct HistogramParameters
{
    std::string Name;       // names of the histograms
    std::string Particle;   // particles being histed
    std::string Type;       // the output paramter
    double Time;            // Time at which hists are made
    unsigned int Bins;      // number of bins in histograms
    double MinBin;          // First value of bins 
    double MaxBin;          // last value of bins
};

class FileParser
{
public:
    FileParser(std::string fileName, bool checkOutput = false);

    ~FileParser();

    GeneralParameters GetGeneral() const {return m_general;}

    FieldParameters GetField() const {return m_field;}

    PhysicsParameters GetPhysics() const {return m_physics;}

    std::vector<ParticleParameters> GetParticle() const {return m_particles;}

    std::vector<HistogramParameters> GetHistograms() const {return m_histograms;}

private:

    // Checks that the vital information needed for the simulation 
    // has been set
    void CheckVitals();

    void ReadGeneral();

    void ReadField();

    void ReadPhysics();

    void ReadParticles();

    void ReadHistograms();

private:
    std::vector<std::string> m_sections;
    GeneralParameters m_general;
    FieldParameters m_field;
    PhysicsParameters m_physics;
    std::vector<ParticleParameters> m_particles;
    std::vector<HistogramParameters> m_histograms;

    INIReader* m_reader;
    UnitsSystem* m_units;

    bool m_checkOutput;
    std::ofstream m_checkFile;
};

#endif