#include "../include/Expression.hpp"

#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>
#include <functional>

using singleArgFunction = std::function<long double(long double)>;
using doubleArgFunction = std::function<long double(long double, long double)>;

const std::map<types, singleArgFunction> singles{
    {types::sin, std::sinl},
    {types::cos, std::cosl},
    {types::ln, std::logl},
    {types::exp, std::expl}};

const std::map<types, doubleArgFunction> doubles{
    {types::plus, [](long double a, long double b)
     { return a + b; }},
    {types::minus, [](long double a, long double b)
     { return a - b; }},
    {types::multiplication, [](long double a, long double b)
     { return a * b; }},
    {types::division, [](long double a, long double b)
     { return a / b; }},
    {types::pow, std::powl}};

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
        std::string returnString = s.str();
        while (returnString.back() == '0')
            returnString.pop_back();
        return returnString;
    }
}

long double Constant::evaluate(const std::map<std::string, long double> &args) const
{
    return num;
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

long double Variable::evaluate(const std::map<std::string, long double> &args) const
{
    return args.at(var);
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

long double MonoOperation::evaluate(const std::map<std::string, long double> &args) const
{
    if (exprType == types::brackets)
        return obj->evaluate(args);
    else
    {
        singleArgFunction actFunc = singles.at(exprType);
        return actFunc(obj->evaluate(args));
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

long double BinaryOperation::evaluate(const std::map<std::string, long double> &args) const
{
    doubleArgFunction actFunc = doubles.at(exprType);
    return actFunc(leftObj->evaluate(args), rightObj->evaluate(args));
}

std::ostream &operator<<(std::ostream &s, const Expression &expr)
{
    s << expr.getString();
    return s;
}
