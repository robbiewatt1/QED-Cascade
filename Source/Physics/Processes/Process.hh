#ifndef PROCESS_HH
#define PROCESS_HH

#include "Particle.hh"
#include "ParticleList.hh"
#include "EMField.hh"

class Process
{
public:
	Process(EMField* field, double dt, bool track): m_field(field), m_dt(dt),
        m_track(track), m_importance(false) { };
	
	virtual ~Process(){ 
        delete m_weights;
        delete m_groups;
    };

	void AddImportance(long int samples, std::vector<double> groups,
        std::vector<double> weights)
    {
        m_samples = samples;
        m_nGroups = groups.size();
        m_weights = new double [m_nGroups];
        std::copy(weights.begin(), weights.end(), m_weights);
        m_groups = new double [m_nGroups];
        std::copy(groups.begin(), groups.end(), m_groups);
        m_importance = true;
    };

    virtual void Interact(Particle *part, ParticleList *partList) const = 0;

protected:
	EMField* m_field;
	double m_dt;
	bool m_track;
    bool m_importance;
    unsigned int m_samples;
    unsigned int m_nGroups;
    double* m_weights;
    double* m_groups;
};
#endif