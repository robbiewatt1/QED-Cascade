#ifndef PROCESS_HH
#define PROCESS_HH

#include "Particle.hh"
#include "ParticleList.hh"
#include "EMField.hh"

class Process
{
public:
    Process(EMField* field, double dt, bool track): m_field(field), m_dt(dt), m_track(track){ };
    
    virtual ~Process(){ };

    virtual void Interact(Particle *part, ParticleList *partList) const = 0;

protected:
    EMField* m_field;
    double m_dt;
    bool m_track;
};
#endif