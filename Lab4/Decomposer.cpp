#include "stdafx.h"
#include "Decomposer.h"
#include <math.h>
#include <string>
#include <iostream>


Decomposer::Decomposer()
{
}


Decomposer::~Decomposer()
{
}


void Decomposer::decompose(unsigned long long num) {
	Decomposition.clear();
	unsigned long long sqrtNum = sqrt(num);
	for (unsigned long long i = 2; i <= sqrtNum; i++) {
		while (num % i == 0) {
			Decomposition.push_back(i);
			num /= i;
		}
	}
	if (num != 1) {
		Decomposition.push_back(num);
}
}


std::string Decomposer::generate_string() {
	std::string ResultString = "";
	for (auto i : Decomposition) {
		ResultString += std::to_string(i) + " * ";
	}
	ResultString.pop_back();
	ResultString.pop_back();
	return ResultString;
}


void Decomposer::compose(std::vector <unsigned long long> decomp) {
	for (auto i : decomp) {
		Number *= i;
	}
}


std::vector <unsigned long long> Decomposer::get_decomposition() {
	return Decomposition;
}


unsigned long long Decomposer::get_number() {
	return Number;
}