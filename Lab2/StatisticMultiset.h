#include <map>
#include <set>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <limits>


template<typename T>
class StatisticMultiset {
public:
	StatisticMultiset();
	~StatisticMultiset();

	void AddNum(T);

	T GetMax() const;
	T GetMin() const;
	float GetAvg() const;
	int CountNums() const;
	int GetCountUnder(float threshold) const;
	int GetCountAbove(float threshold) const;

	void  AddNums(const std::multiset<T>& numbers);
	void  AddNums(const std::vector<T>& numbers);
	void  AddNums(const std::list<T>& numbers);
	void  AddNums(const StatisticMultiset& a_stat_set);
	void  AddNumsFromFile(const char* filename);

	void reset();

private:
	std::multiset<T> set_of_numbers;
	mutable std::pair<T, int> Max;
	mutable std::pair<T, int> Min;
	mutable std::pair<float, int> Avg;
	mutable std::pair<int, int> Count;
	mutable std::pair<std::map<float, int>, int> CountUnder;
	mutable std::pair<std::map<float, int>, int> CountAbove;
};


template<typename T>
StatisticMultiset<T>::StatisticMultiset() {
	Max = std::make_pair(std::numeric_limits<T>::min(), 0);
	Min = std::make_pair(std::numeric_limits<T>::max(), 0);
	Avg = std::make_pair(0, 0);
	std::map<float, int> und;
	std::map<float, int> abv;
	CountUnder = std::make_pair(und, 0);
	CountAbove = std::make_pair(abv, 0);
}

template<typename T>
StatisticMultiset<T>::~StatisticMultiset() {
	CountUnder.first.clear();
	CountAbove.first.clear();
}

template<typename T>
void StatisticMultiset<T>::reset() {
	CountUnder.second = 0;
	CountUnder.first.clear();
	CountAbove.second = 0;
	CountAbove.first.clear();
	Max.second = 0;
	Min.second = 0;
	Avg.second = 0;
	Count.second = 0;
}

template<typename T>
void StatisticMultiset<T>::AddNum(T num) {
	set_of_numbers.insert(num);
	reset();
}

template<typename T>
int StatisticMultiset<T>::CountNums() const {
	if (!Count.second) {
		Count.first = set_of_numbers.size();
		Count.second = 1;
	}
	return Count.first;
}

template<typename T>
T StatisticMultiset<T>::GetMax() const {
	if (!Max.second) {
		for (auto i : set_of_numbers) {
			if (i > Max.first) {
				Max.first = i;
			}
		}
		Max.second = 1;
	}
	return Max.first;
}

template<typename T>
T StatisticMultiset<T>::GetMin() const {
	if (!Min.second) {
		for (auto i : set_of_numbers) {
			if (i < Min.first) {
				Min.first = i;
			}
		}
		Min.second = 1;
	}
	return Min.first;
}

template<typename T>
float StatisticMultiset<T>::GetAvg() const {
	if (!Avg.second) {
		float sum = 0;
		for (auto i : set_of_numbers) {
			sum += i;
		}
		Avg.first = sum / set_of_numbers.size();
		Avg.second = 1;
	}
	return Avg.first;
}

template<typename T>
int StatisticMultiset<T>::GetCountUnder(float threshold) const {
	if (!CountUnder.second) {
		int count = 0;
		for (auto i : set_of_numbers) {
			if (i < threshold) {
				count++;
			}
		}
		CountUnder.first[threshold] = count;
		CountUnder.second = 1;
	}
	else if (!CountUnder.first[threshold]) {
		int count = 0;
		for (auto i : set_of_numbers) {
			if (i < threshold) {
				count++;
			}
		}
		CountUnder.first[threshold] = count;
	}

	return CountUnder.first[threshold];
}

template<typename T>
int StatisticMultiset<T>::GetCountAbove(float threshold) const {
	if (!CountAbove.second) {
		int count = 0;
		for (auto i : set_of_numbers) {
			if (i > threshold) {
				count++;
			}
		}
		CountAbove.first[threshold] = count;
		CountAbove.second = 1;
	}
	else if (!CountAbove.first[threshold]) {
		int count = 0;
		for (auto i : set_of_numbers) {
			if (i > threshold) {
				count++;
			}
		}
		CountAbove.first[threshold] = count;
	}

	return CountAbove.first[threshold];
}

template<typename T>
void StatisticMultiset<T>::AddNums(const std::multiset<T>& numbers) {
	for (auto i : numbers) {
		set_of_numbers.insert(i);
	}
	reset();
}

template<typename T>
void StatisticMultiset<T>::AddNums(const std::vector<T>& numbers) {
	for (int i = 0; i < numbers.size(); i++) {
		set_of_numbers.insert(numbers[i]);
	}
	reset();
}

template<typename T>
void StatisticMultiset<T>::AddNums(const std::list<T>& numbers) {
	for (auto i : numbers) {
		set_of_numbers.insert(i);
	}
	reset();
}

template<typename T>
void StatisticMultiset<T>::AddNumsFromFile(const char* filename) {
	std::string filepath = "D://oop//lab2//";
	std::ifstream fin(filepath + filename);

	T numb;
	while (fin >> numb) {
		set_of_numbers.insert(numb);
	}
	reset();
}

template<typename T>
void StatisticMultiset<T>::AddNums(const StatisticMultiset& a_stat_set) {
	for (auto i : a_stat_set.set_of_numbers) {
		set_of_numbers.insert(i);
	}
	reset();
}

