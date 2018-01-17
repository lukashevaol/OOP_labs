#include <string>
#include <map>
#include <string>
#include <fstream>


class IniParser
{
public:
	IniParser();
	~IniParser();

	// Opens Ini file.    
	void Initialize(const char* filename_cstr);

	// Checks if a section exists.     
	bool IsHaveSection(const char* section_name);

	// Checks if a pair param-section exists.     
	bool IsHaveParam(const char* section_name, const char* param_name);

	// Returns integer value for a pair param-section.     
	int GetValueInt(const char* section_name, const char* param_name);

	// Returns double value for a pair param-section.     
	double GetValueDouble(const char* section_name, const char* param_name);

	// Return string value for a pair param-section.     
	std::string GetValueString(const char* section_name, const char* param_name);

protected:
	std::map <std::string, std::map<std::string, std::string>> file_content;
	std::ifstream file_reading;
	std::string directory = "D://oop//lab1//Ini_Examples//";
};