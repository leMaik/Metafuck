#ifndef EXPRESSION_STRING_H
#define EXPRESSION_STRING_H

#include "Argument.h"
#include <string>

class ExpressionString : public Argument
{
public:
	ExpressionString(std::string expression);
	const std::string expression;

	Argument::Type getType() const;
};

#endif