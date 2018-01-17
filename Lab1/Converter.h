#include <string>


class Converter
{
public:
	Converter();
	~Converter();

	static int ConvertToInt(std::string string_to_convert);
	static double ConvertToDouble(std::string string_to_convert);
};
