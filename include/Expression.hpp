#pragma once
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <memory>
#include <ostream>
#include <map>

enum class types
{
	plus,
	minus,
	multiplication,
	division,
	pow,
	brackets,
	sin,
	cos,
	ln,
	exp,
	other
};

class Expression
{
public:
	virtual ~Expression() = default;
	virtual std::string getString() const = 0;
	virtual long double evaluate(const std::map<std::string, long double> *args = nullptr) const = 0;
	virtual std::shared_ptr<Expression> differentiate(const std::string &) const = 0;

	friend std::shared_ptr<Expression> parse(const std::string &);
	friend std::ostream &operator<<(std::ostream &, const Expression &);

protected:
	types exprType;

	Expression() = default;
	Expression(const Expression &) = default;
	Expression(Expression &&) = default;
	Expression &operator=(const Expression &) = default;
	Expression &operator=(Expression &&) = default;
};

std::shared_ptr<Expression> operator+(std::shared_ptr<Expression>, std::shared_ptr<Expression>);
std::shared_ptr<Expression> operator-(std::shared_ptr<Expression>, std::shared_ptr<Expression>);
std::shared_ptr<Expression> operator*(std::shared_ptr<Expression>, std::shared_ptr<Expression>);
std::shared_ptr<Expression> operator/(std::shared_ptr<Expression>, std::shared_ptr<Expression>);
std::shared_ptr<Expression> operator^(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

class Constant : public Expression
{
private:
	long double num;

public:
	Constant(const char *);
	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
};

class Variable : public Expression
{
private:
	std::string var;

public:
	Variable(std::string strVar);
	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
};

class MonoOperation : public Expression
{
private:
	std::shared_ptr<Expression> obj;

public:
	MonoOperation(types oper, std::shared_ptr<Expression> other);
	friend std::shared_ptr<Expression> parse(const std::string &);
	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
};

class BinaryOperation : public Expression
{
private:
	std::shared_ptr<Expression> leftObj;
	std::shared_ptr<Expression> rightObj;

public:
	BinaryOperation(types oper, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
	friend std::shared_ptr<Expression> parse(const std::string &);
	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
};

#endif