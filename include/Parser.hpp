#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include<memory>

#include"Expression_Impl.hpp"

std::shared_ptr<ExprImpl> parse(const std::string&);

#endif