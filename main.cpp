#include <iostream>
#include <memory>
#include <vector>

#include "include/Expression.hpp"
#include "include/Parser.hpp"

using namespace std;

int main()
{
	std::string tr = "exp(3)-2*x*y^5^(23-x)";
	std::shared_ptr<Expression> res = parse(tr);
	cout << *res << std::endl;
	return 0;
}