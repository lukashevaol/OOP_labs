#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "IniParser.h"

int main()
{
	IniParser parser;
	try {
	parser.Initialize("lab01_input_correct_09.ini");
	//std::cout << parser.GetValueInt("COMMON", "ListenTcpPort1111") << std::endl;
	std::cout << parser.GetValueDouble("DEMODS", "BufferLenSeconds") << std::endl;
	}
	catch (std::runtime_error ex) {
	std::cout << "Error: " << ex.what() << std::endl;
	}
	catch (std::logic_error ex) {
	std::cout << "Error: " << ex.what() << std::endl;
	}

	try {
	parser.Initialize("lab01_input_correct_09999.ini");
	}
	catch (std::runtime_error ex) {
		std::cout << "\nError: " << ex.what() << std::endl;
	}

	try {
		parser.Initialize("lab01_input_correct_09999.init");
	}
	catch (std::runtime_error ex) {
		std::cout << "\nError: " << ex.what() << std::endl;
	}

	system("pause");
	return 0;
}