#include "../include/Expression.hpp"
#include "../include/Parser.hpp"

#include <algorithm>

Expression::Expression(std::shared_ptr<ExprImpl> ptr) : expr(ptr) {}

Expression::Expression(const char *strExpr)
{
    std::string strExprReducted = std::string(strExpr);
    strExprReducted.erase(std::remove(strExprReducted.begin(), strExprReducted.end(), ' '), strExprReducted.end());
    expr = parse(strExprReducted.c_str());
}

Expression::Expression(long double arg) : expr(std::make_shared<Constant>(std::to_string(arg).c_str())) {}

Expression::Expression(const Expression &other) : expr(other.expr->clone()) {}

Expression &Expression::operator=(const Expression &other)
{
    if (this != &other)
        expr = other.expr->clone();
    return *this;
}

Expression Expression::operator+(const Expression &other) const
{
    std::shared_ptr<ExprImpl> res = expr + other.expr;
    return Expression(res);
}

Expression Expression::operator-(const Expression &other) const
{
    std::shared_ptr<ExprImpl> res = expr - other.expr;
    return Expression(res);
}

Expression Expression::operator*(const Expression &other) const
{
    std::shared_ptr<ExprImpl> res = expr * other.expr;
    return Expression(res);
}

Expression Expression::operator/(const Expression &other) const
{
    std::shared_ptr<ExprImpl> res = expr / other.expr;
    return Expression(res);
}

Expression Expression::operator^(const Expression &other) const
{
    std::shared_ptr<ExprImpl> res = expr ^ other.expr;
    return Expression(res);
}

Expression sin(const Expression &expr)
{
    const std::string exprString = expr.getString();
    auto newArg = "sin(" + exprString + ")";
    return Expression(newArg.c_str());
}

Expression cos(const Expression &expr)
{
    const std::string exprString = expr.getString();
    auto newArg = "cos(" + exprString + ")";
    return Expression(newArg.c_str());
}

Expression ln(const Expression &expr)
{
    const std::string exprString = expr.getString();
    auto newArg = "ln(" + exprString + ")";
    return Expression(newArg.c_str());
}

Expression exp(const Expression &expr)
{
    const std::string exprString = expr.getString();
    auto newArg = "exp(" + exprString + ")";
    return Expression(newArg.c_str());
}

std::string Expression::getString() const
{
    return expr->getString();
}

long double Expression::evaluate(const std::map<std::string, long double> *args) const
{
    return expr->evaluate(args);
}

Expression Expression::differentiate(const std::string &arg) const
{
    return Expression(expr->differentiate(arg));
}

std::ostream &operator<<(std::ostream &s, const Expression &other)
{
    s << other.getString();
    return s;
}