#include <iostream>
#include <memory>
#include <vector>

#include "include/Expression.hpp"
#include "include/Parser.hpp"

using namespace std;

int main()
{
	std::map<std::string, long double> myMap{{"x", 5.0l}};
	std::string tr = "sin(cos(sin(x)))*ln(x^3)";
	std::shared_ptr<Expression> res = parse(tr);
	cout << res->evaluate(&myMap) << std::endl;
	return 0;
}