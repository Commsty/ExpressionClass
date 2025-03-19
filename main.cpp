#include <iostream>
#include <memory>
#include <vector>

#include "include/Expression.hpp"
#include "include/Parser.hpp"

using namespace std;

int main()
{
	std::map<std::string, long double> myMap{{"x", 3.14l}};
	std::string tr = "sin(exp(3)*x)-x^8+exp(x^2+2)";
	std::string abs="x";
	std::shared_ptr<Expression> res = parse(tr);
	cout << *res->differentiate(abs) << std::endl;
	return 0;
}