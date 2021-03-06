#include "stdafx.h"
#include <iostream>
#include "StatisticMultiset.h"
#include <set>	
#include <vector>
#include <list>


int main()
{
	StatisticMultiset<int> ms1;
	ms1.AddNum(89);

	StatisticMultiset<int> ms2;
	std::vector<int> somedata = { 10, 40, 6, 87 };
	ms2.AddNums(somedata);

	StatisticMultiset<int> ms3;
	ms3.AddNums(ms1);
	ms3.AddNums(ms2);
	std::cout << "Min: " << ms3.GetMin()
		<< " Avg: " << ms3.GetAvg()
		<< " Max: " << ms3.GetMax()
		<< std::endl;
	std::cout << ms3.GetCountUnder(39) << std::endl;
	std::cout << ms3.GetCountAbove(39) << std::endl;
	std::cout << ms3.CountNums() << std::endl;

	ms3.AddNumsFromFile("test.txt");
	std::cout << ms3.CountNums() << std::endl;

	system("pause");
	return 0;
}