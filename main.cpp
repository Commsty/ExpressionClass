#include <iostream>

#include "include/Expression.hpp"

using namespace std;
 
int main()
{
	std::map<std::string, long double> myMap{{"x", 757.3l},{"y",5.0l}};
	Expression t1("(0)");
	Expression t2("exp(sin (cos(3  *x*  y)))");
	cout<<t2.evaluate(&myMap);
}