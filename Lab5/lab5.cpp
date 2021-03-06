#include "stdafx.h"
#include <thread>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <string>
#include "Decomposer.h"


std::condition_variable cvReadingWriting;
std::condition_variable cvCalculation;
std::mutex mtx;
bool _ready = false;
bool _running = true;
bool _calculationState = false;
Decomposer d;

void change_states() {
	while (true) {
		std::string Input;

		std::cout << "Tap here: "; std::cin >> Input;
		std::cin.clear();

		if (Input == "resume") {
			std::lock_guard<std::mutex> lock(mtx);
			_ready = true;
			cvReadingWriting.notify_one();
		}
		if (Input == "pause") {
			_ready = false;
		}
		if (Input == "exit") {
			_running = false;
		}
	}
};

void calculate(unsigned long long &num, std::string &s) {
	while (true) {
		std::unique_lock<std::mutex> lock(mtx);
		cvCalculation.wait(lock, [] { return _calculationState; });

		_ready = true;
		d.decompose(num);
		s = d.generate_string();

		_calculationState = false;
		cvReadingWriting.notify_one();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}


int main() {
	std::ifstream readFile("..//input.txt");
	std::ofstream writeFile("..//output.txt");

	unsigned long long num;
	std::string s;

	std::thread t1(change_states);
	std::thread t2(calculate, std::ref(num), std::ref(s));

	while (readFile >> num) {
		std::unique_lock<std::mutex> lock(mtx);
		_calculationState = true;
		cvCalculation.notify_one();

		writeFile.close();


		cvReadingWriting.wait(lock, [] { return _ready; });
		_ready = false;

		if (_running == false) {
			break;
		}

		writeFile.open("..//output.txt", std::ios_base::app);
		writeFile << num << ":  " << s << '\n';
	}
	writeFile.close();
	readFile.close();

	t1.detach();
	t2.detach();
	return 0;
}
