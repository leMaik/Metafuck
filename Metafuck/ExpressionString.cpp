#include "ExpressionString.h"

ExpressionString::ExpressionString(std::string expression) : expression(expression)
{
}

Argument::Type ExpressionString::getType() const {
	return Argument::Type::EXPRESSION;
}