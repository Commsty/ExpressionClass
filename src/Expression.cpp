#include "../include/Expression.hpp"
#include"../include/Parser.hpp"

Expression::Expression(std::shared_ptr<ExprImpl> ptr) : expr(ptr) {}

Expression::Expression(const char *strExpr) : expr(parse(strExpr)) {}

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