#include "Particle.hh"


class Lepton: public Particle
{
public:
    Lepton(double mass, double charge, const ThreeVector &position,
           const ThreeVector &momentum, double weight = 1, double time = 0,
           bool tracking = false);


    Lepton(double mass, double charge, double energy, const ThreeVector &position,
           const ThreeVector &direction, double weight = 1, double time = 0,
           bool tracking = false);

    ~Lepton();

    double GetGamma() const override;

    double GetBeta() const override;

    double GetEnergy() const override;

    std::string GetType() const override {return "Lepton";}

    std::string GetName() const override {return m_name;}

private:
    std::string m_name;
};