#include <iostream>

#include "include/Expression.hpp"

int main(int argc, char *argv[])
{
	if (argc < 3)
		throw std::invalid_argument("Incorrect differentiator arguments.");

	if (std::string(argv[1]) == "--diff" && argc == 5)
	{
		const char *strExpr = argv[2];
		if (std::string(argv[3]) == "--by")
		{
			const char *argName = argv[4];
			Expression actExpr(strExpr);
			std::cout << actExpr.differentiate(argName);
		}
		else
			throw std::invalid_argument("Incorrect differentiator arguments.");
	}

	else if (std::string(argv[1]) == "--diff" && argc != 5)
		throw std::invalid_argument("Incorrect differentiator arguments.");

	else if (std::string(argv[1]) == "--eval")
	{
		const char *strExpr = argv[2];
		if (argc > 3)
		{
			std::map<std::string, long double> values;
			for (int i = 3; i < argc; i++)
			{
				std::string valString = std::string(argv[i]);
				size_t index = valString.find_first_of('=');
				if (index != std::string::npos)
				{
					auto argName = valString.substr(0, index);
					auto argVal = valString.substr(index + 1);
					values[argName] = std::strtold(argVal.c_str(), nullptr);
				}
				else
					throw std::invalid_argument("Incorrect differentiator arguments.");
			}
			Expression actExpr(strExpr);
			std::cout << actExpr.evaluate(&values);
		}
		else
		{
			Expression actExpr(strExpr);
			std::cout << actExpr.evaluate(nullptr);
		}
	}
}