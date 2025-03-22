#pragma once
#ifndef EXPR_IMPL_HPP
#define EXPR_IMPL_HPP

#include <string>
#include <memory>
#include <ostream>
#include <map>

class DivisionByZeroError : public std::exception
{
public:
	const char *what() const noexcept;
};

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

class ExprImpl
{
public:
	virtual ~ExprImpl() = default;
	virtual std::string getString() const = 0;
	virtual long double evaluate(const std::map<std::string, long double> *args = nullptr) const = 0;
	virtual std::shared_ptr<ExprImpl> differentiate(const std::string &) const = 0;
	virtual std::shared_ptr<ExprImpl> clone() const = 0;

	friend std::shared_ptr<ExprImpl> parse(const std::string &);
	friend void beautify(std::shared_ptr<ExprImpl> &);

protected:
	types exprType;

	ExprImpl() = default;
	ExprImpl(const ExprImpl&) = default;
    ExprImpl(ExprImpl&&) = default;
    ExprImpl& operator=(const ExprImpl&) = default;
    ExprImpl& operator=(ExprImpl&&) = default;
};

std::shared_ptr<ExprImpl> operator+(std::shared_ptr<ExprImpl>, std::shared_ptr<ExprImpl>);
std::shared_ptr<ExprImpl> operator-(std::shared_ptr<ExprImpl>, std::shared_ptr<ExprImpl>);
std::shared_ptr<ExprImpl> operator*(std::shared_ptr<ExprImpl>, std::shared_ptr<ExprImpl>);
std::shared_ptr<ExprImpl> operator/(std::shared_ptr<ExprImpl>, std::shared_ptr<ExprImpl>);
std::shared_ptr<ExprImpl> operator^(std::shared_ptr<ExprImpl>, std::shared_ptr<ExprImpl>);

class Constant : public ExprImpl
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
	std::shared_ptr<ExprImpl> differentiate(const std::string &) const override;
	std::shared_ptr<ExprImpl> clone() const override;

	friend void beautify(std::shared_ptr<ExprImpl> &);
};

class Variable : public ExprImpl
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
	std::shared_ptr<ExprImpl> differentiate(const std::string &) const override;
	std::shared_ptr<ExprImpl> clone() const override;

	friend void beautify(std::shared_ptr<ExprImpl> &);
};

class MonoOperation : public ExprImpl
{
private:
	std::shared_ptr<ExprImpl> obj;

public:
	MonoOperation(types oper, std::shared_ptr<ExprImpl> other);
	MonoOperation(const MonoOperation &);
	MonoOperation(MonoOperation &&) = default;
	MonoOperation &operator=(const MonoOperation &);
	MonoOperation &operator=(MonoOperation &&) = default;

	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<ExprImpl> differentiate(const std::string &) const override;
	std::shared_ptr<ExprImpl> clone() const override;

	friend std::shared_ptr<ExprImpl> parse(const std::string &);
	friend void beautify(std::shared_ptr<ExprImpl> &);
};

class BinaryOperation : public ExprImpl
{
private:
	std::shared_ptr<ExprImpl> leftObj;
	std::shared_ptr<ExprImpl> rightObj;

public:
	BinaryOperation(types oper, std::shared_ptr<ExprImpl> left, std::shared_ptr<ExprImpl> right);
	BinaryOperation(const BinaryOperation &);
	BinaryOperation(BinaryOperation &&) = default;
	BinaryOperation &operator=(const BinaryOperation &);
	BinaryOperation &operator=(BinaryOperation &&) = default;

	std::string getString() const override;
	long double evaluate(const std::map<std::string, long double> *) const override;
	std::shared_ptr<ExprImpl> differentiate(const std::string &) const override;
	std::shared_ptr<ExprImpl> clone() const override;

	friend std::shared_ptr<ExprImpl> parse(const std::string &);
	friend void beautify(std::shared_ptr<ExprImpl> &);
};

#endif