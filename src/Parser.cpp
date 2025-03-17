#include "../include/Parser.hpp"
#include "../include/Expression.hpp"

#include <vector>
#include <map>
#include <regex>

namespace
{
	const std::map<types, int> typeToPriority{
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

	std::shared_ptr<Expression> myExpr;
	std::shared_ptr<Expression> last;

	for (size_t i = 0; i < tokens->size(); i++)
	{
		auto actToken = (*tokens)[i];
		if (getStringType(actToken) == types::other)
		{
			if (isNumberString(actToken))
			{
				if (!last)
					myExpr = std::make_shared<Constant>(actToken.c_str());
				else
				{
					if (auto monoPtr = dynamic_cast<MonoOperation *>(last.get()))
						monoPtr->obj = std::make_shared<Constant>(actToken.c_str());

					else if (auto binPtr = dynamic_cast<BinaryOperation *>(last.get()))
						binPtr->rightObj = std::make_shared<Constant>(actToken.c_str());
				}
			}
			else if (actToken[0] == '(')
			{
				const std::string underStr = actToken.substr(1, actToken.size() - 2);
				if (!last)
					myExpr = std::make_shared<MonoOperation>(types::brackets, parse(underStr));
				else
				{
					if (auto monoPtr = dynamic_cast<MonoOperation *>(last.get()))
						monoPtr->obj = std::make_shared<MonoOperation>(types::brackets, parse(underStr));

					else if (auto binPtr = dynamic_cast<BinaryOperation *>(last.get()))
						binPtr->rightObj = std::make_shared<MonoOperation>(types::brackets, parse(underStr));
				}
			}
			else
			{
				if (!last)
					myExpr = std::make_shared<Variable>(actToken);
				else
				{
					if (auto monoPtr = dynamic_cast<MonoOperation *>(last.get()))
						monoPtr->obj = std::make_shared<Variable>(actToken);

					else if (auto binPtr = dynamic_cast<BinaryOperation *>(last.get()))
						binPtr->rightObj = std::make_shared<Variable>(actToken);
				}
			}
		}
		else
		{
			types actType = getStringType(actToken);
			std::shared_ptr<Expression> actFunc;

			if (actType == types::sin || actType == types::cos || actType == types::ln || actType == types::exp)
			{
				actFunc = std::make_shared<MonoOperation>(actType, nullptr);
				if (!last)
					myExpr = actFunc;
				else
				{
					if (auto monoPtr = dynamic_cast<MonoOperation *>(last.get()))
						monoPtr->obj = actFunc;

					else if (auto binPtr = dynamic_cast<BinaryOperation *>(last.get()))
						binPtr->rightObj = actFunc;
				}
				last = actFunc;
			}
			else
			{
				int actPriority = typeToPriority.at(actType);
				std::shared_ptr<Expression> base = myExpr, prev;

				while (actPriority >= typeToPriority.at(base->exprType))
				{
					prev = base;
					if (auto monoPtr = dynamic_cast<MonoOperation *>(base.get()))
						base = monoPtr->obj;
					else if (auto binPtr = dynamic_cast<BinaryOperation *>(base.get()))
						base = binPtr->rightObj;
				}

				if (!prev)
				{
					auto leftArg = myExpr;
					myExpr = std::make_shared<BinaryOperation>(actType, leftArg, nullptr);
					last = myExpr;
				}
				else
				{
					actFunc = std::make_shared<BinaryOperation>(actType, base, nullptr);
					auto binPtr = dynamic_cast<BinaryOperation *>(prev.get());
					binPtr->rightObj = actFunc;
					last = actFunc;
				}
			}
		}
	}

	return myExpr;
}