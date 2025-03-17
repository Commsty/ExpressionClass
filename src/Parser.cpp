#include "../include/Parser.hpp"
#include "../include/Expression.hpp"

#include <vector>
#include <map>
#include <regex>

namespace
{
	const std::map<types, int> priority{
		{types::plus, 1},
		{types::minus, 1},
		{types::multiplication, 2},
		{types::division, 2},
		{types::pow, 3},
		{types::brackets, 4},
		{types::sin, 4},
		{types::cos, 4},
		{types::ln, 4},
		{types::exp, 4},
		{types::other, 4}};

	const std::map<std::string, types> strToType{
		{"+", types::plus},
		{"-", types::minus},
		{"*", types::multiplication},
		{"/", types::division},
		{"^", types::pow},
		{"sin", types::sin},
		{"cos", types::cos},
		{"ln", types::ln},
		{"exp", types::exp}};

	const std::shared_ptr<std::vector<std::string>> tokenize(std::string toTokenize)
	{
		auto data = toTokenize.begin();
		auto resultTokens = std::make_shared<std::vector<std::string>>();
		std::string actCommand = "";
		while (data != toTokenize.end())
		{
			if (*data == '(')
			{
				if (!actCommand.empty())
					resultTokens->push_back(actCommand);
				actCommand.clear();
				int cnt = 1;
				actCommand += *data;
				data++;
				while (data != toTokenize.end() && cnt != 0)
				{
					(*data == '(') ? cnt++ : 0;
					(*data == ')') ? cnt-- : 0;
					actCommand += *data;
					data++;
				}
			}
			else
			{
				actCommand += *data;
				data++;
			}

			if (std::string("+-*/^").find(*data) != std::string::npos)
			{
				resultTokens->push_back(actCommand);
				actCommand.clear();
				resultTokens->push_back(std::string(1, *data));
				data++;
			}

			if (data == toTokenize.end())
				resultTokens->push_back(actCommand);
		}
		return resultTokens;
	}

	types getStringType(std::string input)
	{
		auto it = strToType.find(input);
		return (it != strToType.end()) ? it->second : types::other;
	}

	bool isNumberString(const std::string &input)
	{
		return std::regex_match(input, std::regex(R"(\d+\.?\d*)"));
	}
};

std::shared_ptr<Expression> parse(const std::string &data)
{
	auto tokens = tokenize(data);
	types identFirst = getStringType((*tokens)[0]);
	std::shared_ptr<Expression> myExpr;
	int start = 0;

	if (identFirst == types::other)
	{
		
	}

	return myExpr;
}