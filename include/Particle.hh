

class Particle
{
public:
	
	Particle();
	
	~Particle();
	
	std::vector<double> GetPosition() const {return m_position};

	std::vector<double> GetVelocity() const {return m_vecolity;};

	double GetMass() const {return m_mass;}

	double GetCharge() const {return m_charge;}

private:
	double m_mass;	//mass normilised to electron mass
	double m_charge;	//charge normilised to -electron charge

	std::vector<double> m_position;	// current position of particle
	std::vector<double> m_vecolity;	// current velcoity of particle

	std::vector<std::vector<double>> m_posHistory;	// Only used if tracking is on
	std::vector<std::vector<double>> m_velHistory;	// only used if tracking is on
};