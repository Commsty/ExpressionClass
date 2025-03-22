#include "../include/Expression_Impl.hpp"

#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>
#include <functional>

const char *DivisionByZeroError::what() const noexcept
{
	return "Division by zero is not allowed!";
}

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

std::shared_ptr<ExprImpl> operator+(std::shared_ptr<ExprImpl> argLeft, std::shared_ptr<ExprImpl> argRight)
{
    beautify(argLeft);
    beautify(argRight);
    std::shared_ptr<ExprImpl> temp = std::make_shared<BinaryOperation>(types::plus, argLeft, argRight);
    beautify(temp);
    return temp;
}

std::shared_ptr<ExprImpl> operator-(std::shared_ptr<ExprImpl> argLeft, std::shared_ptr<ExprImpl> argRight)
{
    beautify(argLeft);
    beautify(argRight);
    std::shared_ptr<ExprImpl> temp = std::make_shared<BinaryOperation>(types::minus, argLeft, argRight);
    beautify(temp);
    return temp;
}

std::shared_ptr<ExprImpl> operator*(std::shared_ptr<ExprImpl> argLeft, std::shared_ptr<ExprImpl> argRight)
{
    beautify(argLeft);
    beautify(argRight);
    std::shared_ptr<ExprImpl> temp = std::make_shared<BinaryOperation>(types::multiplication, argLeft, argRight);
    beautify(temp);
    return temp;
}

std::shared_ptr<ExprImpl> operator/(std::shared_ptr<ExprImpl> argLeft, std::shared_ptr<ExprImpl> argRight)
{
    beautify(argLeft);
    beautify(argRight);
    std::shared_ptr<ExprImpl> temp = std::make_shared<BinaryOperation>(types::division, argLeft, argRight);
    beautify(temp);
    return temp;
}

std::shared_ptr<ExprImpl> operator^(std::shared_ptr<ExprImpl> argLeft, std::shared_ptr<ExprImpl> argRight)
{
    beautify(argLeft);
    beautify(argRight);
    std::shared_ptr<ExprImpl> temp = std::make_shared<BinaryOperation>(types::pow, argLeft, argRight);
    beautify(temp);
    return temp;
}

Constant::Constant(const Constant &other) : ExprImpl(other), num(other.num) {}
Constant &Constant::operator=(const Constant &other)
{
    if (this != &other)
    {
        ExprImpl::operator=(other);
        num = other.num;
    }
    return *this;
}

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

long double Constant::evaluate([[maybe_unused]] const std::map<std::string, long double> *args) const
{
    return num;
}

std::shared_ptr<ExprImpl> Constant::differentiate([[maybe_unused]] const std::string &arg) const
{
    return std::make_shared<Constant>("0");
}

std::shared_ptr<ExprImpl> Constant::clone() const
{
    return std::make_shared<Constant>(std::to_string(num).c_str());
}

Variable::Variable(const Variable &other) : ExprImpl(other), var(other.var) {}

Variable &Variable::operator=(const Variable &other)
{
    if (this != &other)
    {
        ExprImpl::operator=(other);
        var = other.var;
    }
    return *this;
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

long double Variable::evaluate(const std::map<std::string, long double> *args) const
{
    if (!args)
        return 0.0l;
    try{
        return args->at(var);
    }
    catch(std::out_of_range& e){
        return 0.0l;
    }
}

std::shared_ptr<ExprImpl> Variable::differentiate(const std::string &arg) const
{
    if (var != arg)
        return std::make_shared<Constant>("0");
    return std::make_shared<Constant>("1");
}

std::shared_ptr<ExprImpl> Variable::clone() const
{
    return std::make_shared<Variable>(var);
}

MonoOperation::MonoOperation(const MonoOperation &other) : ExprImpl(other), obj(other.obj->clone()) {}

MonoOperation &MonoOperation::operator=(const MonoOperation &other)
{
    if (this != &other)
    {
        ExprImpl::operator=(other);
        obj = other.obj->clone();
    }
    return *this;
}

MonoOperation::MonoOperation(types oper, std::shared_ptr<ExprImpl> other)
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

long double MonoOperation::evaluate(const std::map<std::string, long double> *args) const
{
    if (exprType == types::brackets)
        return obj->evaluate(args);
    else
    {
        singleArgFunction actFunc = singles.at(exprType);
        if (exprType==types::ln && obj->evaluate(args)<=0)
            throw std::invalid_argument("Incorrect expression. Log argument cannot be non-posotove.");
        return actFunc(obj->evaluate(args));
    }
}

std::shared_ptr<ExprImpl> MonoOperation::differentiate(const std::string &arg) const
{
    std::shared_ptr<ExprImpl> underExpr;
    switch (exprType)
    {
    case types::brackets:
        underExpr = std::make_shared<MonoOperation>(types::brackets, obj->differentiate(arg));
        beautify(underExpr);
        return underExpr;
    case types::sin:
        underExpr = std::make_shared<MonoOperation>(types::cos, obj) * obj->differentiate(arg);
        underExpr = std::make_shared<MonoOperation>(types::brackets, underExpr);
        beautify(underExpr);
        return underExpr;
    case types::cos:
        underExpr = std::make_shared<Constant>("-1") * std::make_shared<MonoOperation>(types::sin, obj) * obj->differentiate(arg);
        underExpr = std::make_shared<MonoOperation>(types::brackets, underExpr);
        beautify(underExpr);
        return underExpr;
    case types::ln:
        underExpr = obj->differentiate(arg) / obj;
        underExpr = std::make_shared<MonoOperation>(types::brackets, underExpr);
        beautify(underExpr);
        return underExpr;
    case types::exp:
        underExpr = std::make_shared<MonoOperation>(*this) * obj->differentiate(arg);
        underExpr = std::make_shared<MonoOperation>(types::brackets, underExpr);
        beautify(underExpr);
        return underExpr;
    default:
        return nullptr;
    }
}

std::shared_ptr<ExprImpl> MonoOperation::clone() const
{
    return std::make_shared<MonoOperation>(exprType, obj->clone());
}

BinaryOperation::BinaryOperation(const BinaryOperation &other)
    : ExprImpl(other), leftObj(other.leftObj->clone()), rightObj(other.rightObj - clone()) {}

BinaryOperation &BinaryOperation::operator=(const BinaryOperation &other)
{
    if (this != &other)
    {
        ExprImpl::operator=(other);
        leftObj = other.leftObj->clone();
        rightObj = other.rightObj->clone();
    }
    return *this;
}

BinaryOperation::BinaryOperation(types oper, std::shared_ptr<ExprImpl> left, std::shared_ptr<ExprImpl> right)
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

long double BinaryOperation::evaluate(const std::map<std::string, long double> *args) const
{
    doubleArgFunction actFunc = doubles.at(exprType);
    return actFunc(leftObj->evaluate(args), rightObj->evaluate(args));
}

std::shared_ptr<ExprImpl> BinaryOperation::differentiate(const std::string &arg) const
{
    std::shared_ptr<ExprImpl> underExpr;

    switch (exprType)
    {
    case types::plus:
        underExpr = leftObj->differentiate(arg) + rightObj->differentiate(arg);
        beautify(underExpr);
        return underExpr;
    case types::minus:
        underExpr = leftObj->differentiate(arg) - rightObj->differentiate(arg);
        beautify(underExpr);
        return underExpr;
    case types::multiplication:
        underExpr = leftObj->differentiate(arg) * rightObj + leftObj * rightObj->differentiate(arg);
        beautify(underExpr);
        return underExpr;
    case types::division:
        underExpr = std::make_shared<MonoOperation>(types::brackets, leftObj->differentiate(arg) * rightObj - leftObj * rightObj->differentiate(arg));
        underExpr = underExpr / (rightObj ^ std::make_shared<Constant>("2"));
        beautify(underExpr);
        return underExpr;
    case types::pow:
        underExpr = std::make_shared<MonoOperation>(types::brackets, rightObj - std::make_shared<Constant>("1"));
        underExpr = rightObj * std::make_shared<BinaryOperation>(types::pow, leftObj, underExpr);
        beautify(underExpr);
        return underExpr;
    default:
        return nullptr;
    }
}

std::shared_ptr<ExprImpl> BinaryOperation::clone() const
{
    return std::make_shared<BinaryOperation>(exprType, leftObj->clone(), rightObj->clone());
}

void beautify(std::shared_ptr<ExprImpl> &exprPtr)
{
    auto ptrMono = dynamic_cast<MonoOperation *>(exprPtr.get());
    if (ptrMono)
    {
        Constant *ptrObj = dynamic_cast<Constant *>(ptrMono->obj.get());
        switch (ptrMono->exprType)
        {
        case types::sin:
            if (ptrObj && fabsl(ptrObj->num) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("0");
            break;
        case types::cos:
            if (ptrObj && fabsl(ptrObj->num) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("1");
            break;
        case types::ln:
            if (ptrObj && fabsl(ptrObj->num - 1.0l) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("0");
            break;
        case types::exp:
            if (ptrObj && fabsl(ptrObj->num) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("1");
            break;
        case types::brackets:
            if (dynamic_cast<Constant *>(ptrMono->obj.get()) || dynamic_cast<Variable *>(ptrMono->obj.get()) || dynamic_cast<MonoOperation *>(ptrMono->obj.get()))
                exprPtr = ptrMono->obj;
            break;
        default:;
        }
    }
    auto ptrBin = dynamic_cast<BinaryOperation *>(exprPtr.get());
    if (ptrBin)
    {
        Constant *ptrObjLeft = dynamic_cast<Constant *>(ptrBin->leftObj.get());
        Constant *ptrObjRight = dynamic_cast<Constant *>(ptrBin->rightObj.get());
        switch (ptrBin->exprType)
        {
        case types::plus:
            if (ptrObjLeft && fabsl(ptrObjLeft->num) <= 0.0000001l)
                exprPtr = ptrBin->rightObj;
            else if (ptrObjRight && fabsl(ptrObjRight->num) <= 0.0000001l)
                exprPtr = ptrBin->leftObj;
            else if (ptrObjLeft && ptrObjRight)
                exprPtr = std::make_shared<Constant>(std::to_string(ptrObjLeft->num + ptrObjRight->num).c_str());
            break;
        case types::minus:
            if (ptrObjRight && fabsl(ptrObjRight->num) <= 0.0000001l)
                exprPtr = ptrBin->leftObj;
            else if (ptrObjLeft && ptrObjRight)
                exprPtr = std::make_shared<Constant>(std::to_string(ptrObjLeft->num - ptrObjRight->num).c_str());
            break;
        case types::multiplication:
            if (ptrObjLeft && fabsl(ptrObjLeft->num) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("0");
            else if (ptrObjRight && fabsl(ptrObjRight->num) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("0");
            else if (ptrObjLeft && fabsl(ptrObjLeft->num - 1.0l) <= 0.0000001l)
                exprPtr = ptrBin->rightObj;
            else if (ptrObjRight && fabsl(ptrObjRight->num - 1.0l) <= 0.0000001l)
                exprPtr = ptrBin->leftObj;
            else if (ptrObjLeft && ptrObjRight)
                exprPtr = std::make_shared<Constant>(std::to_string(ptrObjLeft->num * ptrObjRight->num).c_str());
            break;
        case types::division:
            if (ptrObjLeft && fabsl(ptrObjLeft->num) <= 0.0000001l && ptrObjRight && fabsl(ptrObjRight->num) >= 0.0000001l)
                exprPtr = std::make_shared<Constant>("0");
            else if (ptrObjLeft && fabsl(ptrObjLeft->num) <= 0.0000001l && (!ptrObjRight))
                exprPtr = std::make_shared<Constant>("0");
            else if (ptrObjLeft && ptrObjRight && fabsl(ptrObjRight->num) >= 0.0000001l)
                exprPtr = std::make_shared<Constant>(std::to_string(ptrObjLeft->num / ptrObjRight->num).c_str());
            else if (ptrObjRight && fabsl(ptrObjRight->num) <= 0.0000001l)
                throw DivisionByZeroError();
            break;
        case types::pow:
            if (ptrObjLeft && fabsl(ptrObjLeft->num) <= 0.0000001l && ptrObjRight && fabsl(ptrObjRight->num) >= 0.0000001l)
                exprPtr = exprPtr = std::make_shared<Constant>("0");
            else if (ptrObjLeft && fabsl(ptrObjLeft->num) <= 0.0000001l && (!ptrObjRight))
                exprPtr = std::make_shared<Constant>("0");
            else if (ptrObjLeft && fabsl(ptrObjLeft->num) >= 0.0000001l && ptrObjRight && fabsl(ptrObjRight->num) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("1");
            else if ((!ptrObjLeft) && ptrObjRight && fabsl(ptrObjRight->num) <= 0.0000001l)
                exprPtr = std::make_shared<Constant>("1");
            else if ((!ptrObjLeft) && ptrObjRight && fabsl(ptrObjRight->num - 1.0l) <= 0.0000001l)
                exprPtr = ptrBin->leftObj;
            else if (ptrObjLeft && fabsl(ptrObjLeft->num) >= 0.0000001l && ptrObjRight && fabsl(ptrObjRight->num) >= 0.0000001l)
                exprPtr = std::make_shared<Constant>(std::to_string(std::powl(ptrObjLeft->num, ptrObjRight->num)).c_str());
            break;
        default:;
        }
    }
}