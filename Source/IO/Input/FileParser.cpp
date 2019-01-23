#include <algorithm>
#include "FileParser.hh"

FileParser::FileParser(std::string fileName)
{
	m_reader = new INIReader(fileName);
	m_sections = m_reader->GetSections();
	CheckVitals();
}

FileParser::~FileParser()
{
	delete m_reader;
}

void FileParser::CheckVitals()
{
	if (std::find(m_sections.begin(), m_sections.end(), "General") == m_sections.end())
	{
		std::cerr << "Input error: File does not contain \"General\" field\n";
		std::cerr << "Exiting!\n";
		exit(1);
	} else if (!(m_reader->HasValue("General", "time_step")))
	{
		std::cerr << "Input error: File does not contain \"time_step\" field\n";
		std::cerr << "Exiting!\n";
		exit(1);
	} else if (!(m_reader->HasValue("General", "time_end")))
	{
		std::cerr << "Input error: File does not contain \"time_end\" field\n";
		std::cerr << "Exiting!\n";
		exit(1);
	}
}

void FileParser::ReadField()
{
	m_field.fieldType = m_reader->GetString("Field", "field_type", "");
	if (m_field.fieldType != "guassian" || m_field.fieldType != "plane"
		|| m_field.fieldType != "static")
	 {
	 	std::cerr << "Input error: Field type \"" << m_field.fieldType << "\" is not recognise.\n";
		std::cerr << "Exiting!\n";
		exit(1);
	 } 
	 if (m_field.fieldType == "static")
	 {
	 	m_field.fieldE = m_reader->GetThreeVector("Field", "e_field", ThreeVector(0,0,0));
	 	m_field.fieldB = m_reader->GetThreeVector("Field", "b_field", ThreeVector(0,0,0));
	 } else if (m_field.fieldType == "plane")
	 {
	 	m_field.fieldMaxE = m_reader->

	 }
}