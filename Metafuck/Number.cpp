#include "Number.h"


Number::Number(const std::string code) : value_(std::stol(code))
{
}

Number::Number()
{
}

Number::~Number()
{
}

unsigned int Number::getValue() const {
	return value_;
}

Argument::Type Number::getType() const{
	return Argument::Type::INTEGER;
}
