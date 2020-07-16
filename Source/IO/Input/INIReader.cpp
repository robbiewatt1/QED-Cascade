// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// https://github.com/benhoyt/inih

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <sys/stat.h>
#include "ini.hh"

#include "INIReader.hh"

using std::string;

INIReader::INIReader()
{
}

INIReader::INIReader(const string& filename)
{
    
    struct stat buffer;
    if (stat (filename.c_str(), &buffer) == 0)
    {
        std::cerr << "Error: SFQED input not found." << std::endl;
        exit(1);
    }

    _error = ini_parse(filename.c_str(), ValueHandler, this);
}

int INIReader::ParseError() const
{
    return _error;
}

string INIReader::Get(const string& section, const string& name, const string& default_value) const
{
    string key = MakeKey(section, name);
    // Use _values.find() here instead of _values.at() to support pre C++11 compilers
    return _values.count(key) ? _values.find(key)->second : default_value;
}

string INIReader::GetString(const string& section, const string& name, const string& default_value) const
{
    const string str = Get(section, name, "");
    return str.empty() ? default_value : str;
}

long INIReader::GetInteger(const string& section, const string& name, long default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double INIReader::GetReal(const string& section, const string& name, double default_value) const
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool INIReader::GetBoolean(const string& section, const string& name, bool default_value) const
{
    string valstr = Get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

ThreeVector INIReader::GetThreeVector(const std::string& section, const std::string& name,
                                      const ThreeVector& default_value) const
{
    struct NotNumber
    {
        bool operator()(const char c) 
        {
            if (c == '.' || c == '-' || c == ' ' || c == 'e' || std::isdigit(c))
            {
                return false;
            } else
            {
                return true;
            }
        }
    };

    string valstr = Get(section, name, "");
    NotNumber nn;
    std::string::iterator end = std::remove_if(valstr.begin(), valstr.end(), nn);
    std::string allNumbers(valstr.begin(), end);
    std::stringstream stream(allNumbers);

    ThreeVector vector;
    int i = 0;
    for (double value = 0; stream >> value;)
    {
        if (i == 3)
        {
            std::cerr << "ERROR: Too many values for ThreeVector.\n";
            exit(1);
        }
        vector[i] = value;
        i++;
    }
    return vector;
}

bool INIReader::HasValue(const std::string& section, const std::string& name) const
{
    string key = MakeKey(section, name);
    return _values.count(key);
}

string INIReader::MakeKey(const string& section, const string& name)
{
    string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

std::vector<std::string> INIReader::GetSections() const
{
    return _sections;
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    INIReader* reader = static_cast<INIReader*>(user);
    string key = MakeKey(section, name);
    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value;

    std::vector<std::string>::iterator sec_beg = reader->_sections.begin();
    std::vector<std::string>::iterator sec_end = reader->_sections.end();
    if (std::find(sec_beg, sec_end, section) == reader->_sections.end())
        reader->_sections.push_back(section);
    return 1;
}
