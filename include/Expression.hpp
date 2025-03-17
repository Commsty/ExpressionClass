#pragma once
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <memory>

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
	virtual types getType()
	{
		return exprType;
	}

protected:
	types exprType;
	Expression() = default;
	Expression(const Expression &) = default;
	Expression(Expression &&) = default;
	Expression &operator=(const Expression &) = default;
	Expression &operator=(Expression &&) = default;
};

class Constant : public Expression
{
public:
	long double num;
	Constant(const char *);
};

class Variable : public Expression
{
public:
	std::string var;
	Variable(std::string strVar);
};

class MonoOperation : public Expression
{
private:
	std::shared_ptr<Expression> obj;

public:
	MonoOperation(types oper, std::shared_ptr<Expression> other);
};

class BinaryOperation : public Expression
{
private:
	std::shared_ptr<Expression> leftObj;
	std::shared_ptr<Expression> rightObj;

public:
	BinaryOperation(types oper, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
};

#endif