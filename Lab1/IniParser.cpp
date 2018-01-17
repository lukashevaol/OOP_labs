#include "stdafx.h"
#include "IniParser.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include "Converter.h"


IniParser::IniParser()
{
}


IniParser::~IniParser()
{
	std::string().swap(directory);
	file_content.clear();
	file_reading.close();
}


void IniParser::Initialize(const char* filename) {
	std::string fname = filename;
	int i = fname.find_last_of('.');
	if (fname.substr(i + 1) != "ini") throw std::runtime_error("Incorrect file format");

	file_content.clear();

	file_reading.open(directory + filename);
	if (!file_reading) throw std::runtime_error("File does not exist");

	std::cout << "\nFile " << filename << " has been initialized successfully!" << std::endl;
		
	std::string current_string = "";
	std::map <std::string, std::string> current_section_content;
	std::string current_section = "", current_param = "", current_value = "";
	while (std::getline(file_reading, current_string)) {
		int comment = current_string.find(';');
		if (comment != std::string::npos) {

			current_string.erase(comment);
		}

		if (current_string != "") {
			int position = current_string.find('=');
			if (position != std::string::npos) {
					std::string param = current_string.substr(0, position), value = current_string.substr(position + 1);
				int i = 0;
				while ((i = param.find(' ')) != std::string::npos)
					param.erase(i, 1);
				i = 0;
				while ((i = value.find(' ')) != std::string::npos)
					value.erase(i, 1);
				current_section_content.insert(std::make_pair(param, value));
			}
			else {
				position = current_string.find(']');
				if (position != std::string::npos) {
						if (current_section != "") {
							file_content.insert(std::make_pair(current_section, current_section_content));
						}
					current_section_content.clear();
					current_section = current_string.substr(1, position - 1);
				}
			}
		}
	}
	file_content.insert(std::make_pair(current_section, current_section_content));
	file_reading.close();
}


bool IniParser::IsHaveSection(const char* section_name) {
	return file_content.find(section_name) != file_content.end();
}


bool IniParser::IsHaveParam(const char* section_name, const char* param_name) {
	auto section_content = file_content[section_name];
	return section_content.find(param_name) != section_content.end();
}


int IniParser::GetValueInt(const char* section_name, const char* param_name) {
	if (IsHaveSection(section_name) && IsHaveParam(section_name, param_name)) {
		return Converter::ConvertToInt(file_content[section_name][param_name]);
	}
	else {
		throw std::runtime_error("Requested section-parameter couple does not exist");
	}
}


double IniParser::GetValueDouble(const char* section_name, const char* param_name) {
	if (IsHaveSection(section_name) && IsHaveParam(section_name, param_name)) {
		return Converter::ConvertToDouble(file_content[section_name][param_name]);
	}
	else {
		throw std::runtime_error("Requested section-parameter couple does not exist");
	}
}


std::string IniParser::GetValueString(const char* section_name, const char* param_name) {
	if (IsHaveSection(section_name) && IsHaveParam(section_name, param_name)) {
		return file_content[section_name][param_name];
	}
	else {
		throw std::runtime_error("Requested section-parameter couple does not exist");
	}
}
