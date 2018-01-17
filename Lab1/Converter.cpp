#include "stdafx.h"
#include "Converter.h"
#include <string>
#include <exception>
#include <iostream>


Converter::Converter()
{
}


Converter::~Converter()
{
}

int Converter::ConvertToInt(std::string string_to_convert) {
	bool flag = true;
	for (int i = 0; i < string_to_convert.length(); i++) {
		if ((int)string_to_convert[i] > 57 || (int)string_to_convert[i] < 48) {
			flag = false;
			break;
		}
	}
	if (flag) {
		int result = stoi(string_to_convert);
		return result;
	}
	else {
		throw std::logic_error("Invalid (string to int) argument");
	}
}

double Converter::ConvertToDouble(std::string string_to_convert) {
	int count = 0, pos = -1;
	bool flag = true;
	pos = string_to_convert.find('.');
	while (pos != std::string::npos) {
		count++;
		pos = string_to_convert.find('.', pos + 1);
	}
	if (count <= 1) {
		for (int i = 0; i < string_to_convert.length(); i++) {
			int check = (int)string_to_convert[i];
			if (check > 57 || (check < 48 && check != 46)) {
				flag = false;
				break;
			}
		}
	}
	else {
		flag = false;
	}
	if (flag) {
		double result = stod(string_to_convert);
		return result;
	}
	else {
		throw std::logic_error("Invalid (string to double) argument");
	}
}