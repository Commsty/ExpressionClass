#include "../include/Expression.hpp"

Constant::Constant(const char* strNum)
    : num(strtold(strNum, nullptr))
{
    exprType=types::other;
}

Variable::Variable(std::string strVar)
    : var(strVar)
{
    exprType=types::other;
}

MonoOperation::MonoOperation(types oper, std::shared_ptr<Expression> other)
    : obj(other)
{
    exprType=oper;
}

BinaryOperation::BinaryOperation(types oper, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
    : leftObj(left), rightObj(right)
{
    exprType=oper;
}
