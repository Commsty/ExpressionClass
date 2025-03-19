#include <iostream>

#include "include/Expression.hpp"

using namespace std;
 
int main()
{
	std::map<std::string, long double> myMap{{"x", 0.0l}};
	Expression test("sin(cos(x))");
	cout<<test.evaluate(&myMap);
	return 0;
}