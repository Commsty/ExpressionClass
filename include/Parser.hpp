#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include<memory>

#include"Expression.hpp"

std::shared_ptr<Expression> parse(const std::string&);

#endif