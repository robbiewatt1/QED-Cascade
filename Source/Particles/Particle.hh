#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "ThreeVector.hh"

class Particle
{
public:
    
    Particle();

    Particle(double mass, double charge, double weight = 1, double time = 0,
        bool tracking = false);

    Particle(double mass, double charge, const ThreeVector &position, 
             const ThreeVector &momentum, double weight = 1, double time = 0, 
             bool tracking = false);
    
    virtual ~Particle();
    
    void UpdateTrack(const ThreeVector &position, const ThreeVector &momentum);
    
    void UpdateTime(double dt);

    void UpdateOpticalDepth(double dtau);

    void Kill() {m_isAlive = false;}

    ThreeVector GetPosition() const {return m_position;}

    ThreeVector GetMomentum() const {return m_momentum;}

    ThreeVector GetDirection() const;
    
    double GetMass() const {return m_mass;}

    double GetCharge() const {return m_charge;}

    double GetTime() const {return m_time;}

    double GetWeight() const {return m_weight;}

    double GetOpticalDepth() const {return m_opticalDepth;}

    bool IsAlive() const {return m_isAlive;}

    bool GetTracking() const {return m_tracking;}

    virtual double GetGamma() const = 0;

    virtual double GetBeta() const = 0;

    virtual double GetEnergy() const = 0;

    virtual std::string GetType() const = 0;

    virtual std::string GetName() const = 0;

    std::vector<ThreeVector> GetPosHist() const {return m_posHistory;}

    std::vector<ThreeVector> GetMomHist() const {return m_momHistory;}

    std::vector<double> GetTimeHist() const {return m_timeHistory;}

    std::vector<double> GetGammaHist() const {return m_gammaHistory;}

    void InitOpticalDepth();

protected:
    double m_mass;  //mass normilised to electron mass
    double m_charge;    //charge normilised to -electron charge
    double m_weight;        // particle weight
    double m_time;  // global time of particle
    double m_opticalDepth;  // optical depth of particle
    bool m_tracking;    // If set to true, particle tracking turned on
    bool m_isAlive;     // If set to false the particle will no longer react or move 

    ThreeVector m_position; // current position of particle
    ThreeVector m_momentum; // current velcoity of particle
    std::vector<ThreeVector> m_posHistory;  // Only used if tracking is on
    std::vector<ThreeVector> m_momHistory;  // only used if tracking is on
    std::vector<double> m_timeHistory;
    std::vector<double> m_gammaHistory;
};

#endif