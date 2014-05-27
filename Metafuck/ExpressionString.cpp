#include "ExpressionString.h"

ExpressionString::ExpressionString(std::string expression) : expression(expression)
{
}

Type ExpressionString::getType() const {
	return Type::EXPRESSION;
}
