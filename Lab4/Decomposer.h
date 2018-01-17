#include <vector>
#include <string>

class Decomposer
{
public:
	Decomposer();
	~Decomposer();

	void decompose(unsigned long long num);
	void compose(std::vector <unsigned long long> decomp);
	std::string generate_string();
	std::vector <unsigned long long> get_decomposition();
	unsigned long long get_number();

private:
	std::vector <unsigned long long> Decomposition;
	unsigned long long Number;
};

