#include "stdafx.h"
#include "Decomposer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>


int main(int argc, char * argv[]) {
	if (argc != 3) {
		throw std::invalid_argument("Invalid agruments");
	}

	std::string rf = argv[1];
	std::string wf = argv[2];
	std::ifstream readFile("..//" + rf);
	std::ofstream writeFile("..//" + wf);
	unsigned long long num;
	Decomposer d;
	while (readFile >> num) {
		d.decompose(num);
		std::string s = d.generate_string();
		writeFile << num << ": " << s << '\n';
	}
	
	readFile.close();
	writeFile.close();
}
