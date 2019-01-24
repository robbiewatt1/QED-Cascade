#include "Process.hh"

Process::Process():
m_genSecondary(false), m_secondaries(NULL)
{
}

Process::~Process()
{
}

void Process::SetSecondaryList(ParticleList* partList)
{
	m_genSecondary = true;
	m_secondaries = partList;
}
