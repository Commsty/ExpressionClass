#include <iostream>
#include <memory>
#include <vector>

#include "include/Expression.hpp"
#include "include/Parser.hpp"

using namespace std;

int main()
{
	std::map<std::string, long double> myMap{{"x", 3.14l}};
	std::string tr = "exp(sin(10*x^5+10*x^4+10*x^3))";
	std::shared_ptr<Expression> res = parse(tr);
	cout << res->evaluate(&myMap) << std::endl;
	// cout<<*res;
	return 0;
}