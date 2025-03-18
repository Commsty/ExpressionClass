#include "../include/Expression.hpp"

#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>

Constant::Constant(const char *strNum)
    : num(strtold(strNum, nullptr))
{
    exprType = types::other;
}

std::string Constant::getString() const
{
    long double intPart;
    if (std::modfl(num, &intPart) == 0.0l)
        return std::to_string(static_cast<long long>(intPart));
    else
    {
        std::ostringstream s;
        s << std::fixed << num;
        std::string returnString=s.str();
        while (returnString.back()=='0')
            returnString.pop_back();
        return returnString;
    }
}

Variable::Variable(std::string strVar)
    : var(strVar)
{
    exprType = types::other;
}

std::string Variable::getString() const
{
    return var;
}

MonoOperation::MonoOperation(types oper, std::shared_ptr<Expression> other)
    : obj(other)
{
    exprType = oper;
}

std::string MonoOperation::getString() const
{
    switch (exprType)
    {
    case types::brackets:
        return "(" + obj->getString() + ")";
    case types::sin:
        return "sin" + obj->getString();
    case types::cos:
        return "cos" + obj->getString();
    case types::ln:
        return "ln" + obj->getString();
    case types::exp:
        return "exp" + obj->getString();
    default:
        return "Error during parsing occurred.";
    }
}

BinaryOperation::BinaryOperation(types oper, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
    : leftObj(left), rightObj(right)
{
    exprType = oper;
}

std::string BinaryOperation::getString() const
{
    switch (exprType)
    {
    case types::plus:
        return leftObj->getString() + "+" + rightObj->getString();
    case types::minus:
        return leftObj->getString() + "-" + rightObj->getString();
    case types::multiplication:
        return leftObj->getString() + "*" + rightObj->getString();
    case types::division:
        return leftObj->getString() + "/" + rightObj->getString();
    case types::pow:
        return leftObj->getString() + "^" + rightObj->getString();
    default:
        return "Error during parsing occurred.";
    }
}

std::ostream &operator<<(std::ostream &s, const Expression &expr)
{
    s << expr.getString();
    return s;
}
