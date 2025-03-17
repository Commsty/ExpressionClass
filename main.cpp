#include <iostream>
#include <memory>
#include <vector>

#include"include/Expression.hpp"
#include"include/Parser.hpp"

using namespace std;


int main()
{
	std:: string tr="sin(123)";
	std::shared_ptr<Expression> res=parse(tr);
	
	return 0;
}