#pragma once
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Expression_Impl.hpp"

class DivisionByZeroError;

class Expression
{
private:
    std::shared_ptr<ExprImpl> expr;
    Expression(std::shared_ptr<ExprImpl>);

public:
    Expression(const char *);
    Expression(long double);
    Expression(const Expression &);
    Expression(Expression &&) = default;
    Expression &operator=(const Expression &);
    Expression &operator=(Expression &&) = default;

    Expression operator+(const Expression&) const;
    Expression operator-(const Expression&) const;
    Expression operator*(const Expression&) const;
    Expression operator/(const Expression&) const;
    Expression operator^(const Expression&) const;

    std::string getString() const;
    long double evaluate(const std::map<std::string, long double> * args = nullptr) const;
    Expression differentiate(const std::string&) const;

    friend std::ostream &operator<<(std::ostream &, const Expression &);
};

#endif