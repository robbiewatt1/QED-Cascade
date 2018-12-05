#include <vector>

class Particle
{
public:
	
	Particle();

	Particle(double mass, double charge, bool tracking = false);

	Particle(double mass, double charge, const std::vector<double> &position, 
			 const std::vector<double> &momentum, bool tracking = false);
	
	virtual ~Particle() = 0;
	
	std::vector<double> GetPosition() const {return m_position;}

	std::vector<double> GetMomentum() const {return m_momentum;}

	void UpdatePosition(const std::vector<double> &position);
	
	void UpdateMomentum(const std::vector<double> &velocity);

	double GetMass() const {return m_mass;}

	double GetCharge() const {return m_charge;}

	double GetGamma() const;

private:
	bool m_tracking;	// If set to true, particle tracking turned on
	double m_mass;	//mass normilised to electron mass
	double m_charge;	//charge normilised to -electron charge

	std::vector<double> m_position;	// current position of particle
	std::vector<double> m_momentum;	// current velcoity of particle

	std::vector<std::vector<double>> m_posHistory;	// Only used if tracking is on
	std::vector<std::vector<double>> m_momHistory;	// only used if tracking is on
};