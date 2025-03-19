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
	virtual std::shared_ptr<Expression> clone() const = 0;

	friend std::shared_ptr<Expression> parse(const std::string &);
	friend std::ostream &operator<<(std::ostream &, const Expression &);
	friend void beautify(std::shared_ptr<Expression> &);

protected:
	types exprType;

	Expression() = default;
	Expression(const Expression&) = default;
    Expression(Expression&&) = default;
    Expression& operator=(const Expression&) = default;
    Expression& operator=(Expression&&) = default;
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
	Constant(const Constant &);
	Constant(Constant &&) = default;
	Constant &operator=(const Constant &);
	Constant &operator=(Constant &&) = default;

	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
	std::shared_ptr<Expression> clone() const override;

	friend void beautify(std::shared_ptr<Expression> &);
};

class Variable : public Expression
{
private:
	std::string var;

public:
	Variable(std::string strVar);
	Variable(const Variable &);
	Variable(Variable &&) = default;
	Variable &operator=(const Variable &);
	Variable &operator=(Variable &&) = default;

	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
	std::shared_ptr<Expression> clone() const override;

	friend void beautify(std::shared_ptr<Expression> &);
};

class MonoOperation : public Expression
{
private:
	std::shared_ptr<Expression> obj;

public:
	MonoOperation(types oper, std::shared_ptr<Expression> other);
	MonoOperation(const MonoOperation &);
	MonoOperation(MonoOperation &&) = default;
	MonoOperation &operator=(const MonoOperation &);
	MonoOperation &operator=(MonoOperation &&) = default;

	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
	std::shared_ptr<Expression> clone() const override;

	friend std::shared_ptr<Expression> parse(const std::string &);
	friend void beautify(std::shared_ptr<Expression> &);
};

class BinaryOperation : public Expression
{
private:
	std::shared_ptr<Expression> leftObj;
	std::shared_ptr<Expression> rightObj;

public:
	BinaryOperation(types oper, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
	BinaryOperation(const BinaryOperation &);
	BinaryOperation(BinaryOperation &&) = default;
	BinaryOperation &operator=(const BinaryOperation &);
	BinaryOperation &operator=(BinaryOperation &&) = default;

	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<Expression> differentiate(const std::string &) const override;
	std::shared_ptr<Expression> clone() const override;

	friend std::shared_ptr<Expression> parse(const std::string &);
	friend void beautify(std::shared_ptr<Expression> &);
};

#endif